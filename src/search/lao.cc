#include "lao.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "track.h"
#include "graph.h"
#include "lao.h"
#include "backup.h"
#include "solve.h"
#include "../globals.h"
#include "lao_wrapper.h"        // printAction
#include "graph.h"              // StateNode::expand
#include "vi.h"
#include "lug.h"
#include <iostream>
#include <float.h>
#include <math.h>

using namespace __gnu_cxx;
using namespace std;

LAOStar::LAOStar()
: Search(LAO){
  NumExpandedStates = 0;

  Iteration = 0;
  NumExpandedStatesIter = 0;
  NumAncestorStatesIter = 0;
  NumSolutionStates = 0;
  ExpandedStateList = NULL;
  CountBackups = 0;
  Residual = 0.0;
  ExitFlag = 0;
  converged_to_goal = 0;
  dfs_limit = 0;
  lastBackup = NULL;
}

void LAOStar::search(){
  StateList *AncestorList;
  struct StateNode *node;
  lastBackup = Start;

  dfs_limit = (int)Start->f;
  for(Iteration = 1; ; Iteration++){
    printf("%3d ( %f secs.)", Iteration, (float)(clock()-gStartTime)/CLOCKS_PER_SEC);
    printf("  g: %f, h: %f, p: %f, exp: %d, expc: %d\n",
           lastBackup->g, lastBackup->h, 
           lastBackup->ExtendedGoalSatisfaction,
           NumExpandedStatesIter, NumExpandedStates); fflush(stdout);

    /* Expand best partial solution */
    NumExpandedStatesIter = 0;
    ExpandedStateList = new StateList;

    ExpandSolution(lastBackup, 0, 0, 1.0);  

    /* Call convergence test if solution graph has no unexpanded tip states */
    if(lastBackup->g == DBL_MAX)
      break;

    if(max_horizon == -1 && Start->ExtendedGoalSatisfaction >= goal_threshold && goal_threshold > 0.0)
      break;

    if(((NumExpandedStatesIter == 0) && ConvergenceTest())
    || (allowed_time > -1 && (clock() - gStartTime) * 1000 / CLOCKS_PER_SEC > allowed_time))    // allowing for lcp's
      break;    // prevents anytime algo
    else if(max_horizon == -1 && goal_threshold == 0.0)
      break;

    if(Start->ExtendedGoalSatisfaction == 1.0)
      break;

    /* Dynamic programming step */
    /* (Skip if backups were performed in expansion step) */
    if(gBackupMethod == separate){
      /* Update state costs and best partial solution */
      NumAncestorStatesIter = 0;
      /* the following condition is necessary in case no nodes are
         expanded but the convergence test fails */
      if(NumExpandedStatesIter > 0){
        AncestorList = findAncestors(ExpandedStateList);
        /* note that expanded nodes are included as ancestors */
        ValueIteration(AncestorList, 3);
        delete AncestorList;
      }
    }

    delete ExpandedStateList;
    /* NumSolutionStates includes GOAL and WALL */
  }

  if(Start->ExtendedGoalSatisfaction >= goal_threshold){
    printf("\nf: %f", Start->f);
    printf("  NumSolutionStates: %d  NumExpandedStates: %d\n", NumSolutionStates, NumExpandedStates);
  }
}

/* Returns list of expanded states */
/* if num expanded nodes exceeds limit, break */
void LAOStar::ExpandSolution(struct StateNode *node, int currHorizon, int dfs_depth, double path_pr){
  struct StateDistribution *successor;

  /* If not yet expanded, expand here. Then consider successor nodes for expansion. */

  if(node->Terminal == 1){
    node->prReached = path_pr;
    return;
  }

  double nextCost = 0.0;

  if(!node->dd){
    node->h = DBL_MAX;
    node->g = DBL_MAX;
    node->f = DBL_MAX;
    cout << "no dd for node " << node->StateNo << endl;
    return;
  }

  if(node->Expanded == -1 || (node->Expanded == 0 && (!(OBSERVABILITY==OBS_FULL && Start->BestAction) || node != Start))){
    ExpandNode(node, currHorizon);
    NumExpandedStatesIter++;
    if(DO_INCREMENTAL_HEURISTIC && node->hIncrement <= MAX_H_INCREMENT)
      return;

    LeafStates->remove(node);

    if(gBackupMethod == combine) 
      Backup(node, currHorizon);

    if(node->NextActions == NULL || node->BestAction == NULL){
      cout << "NO ACTION " << node->StateNo << endl;
      node->h = DBL_MAX;
      node->g = DBL_MAX;
      node->f = DBL_MAX;
      return;
    }

    if(!ENUMERATE_SPACE)
     return;
  }

  if(node->BestAction != NULL){
    if(ENUMERATE_SPACE){
      for(ActionNodeList::iterator acts = node->NextActions->begin(); acts != node->NextActions->end(); acts++){
        for(successor = (*acts)->NextState; successor != NULL; successor = successor->Next){
          /* Stop if already visited this iteration or goal state */
          if(successor->State->Expanded < Iteration){
            /* If already expanded, just mark as visited this iteration */
            if(successor->State->Expanded > 0)
              successor->State->Expanded = Iteration;

            ExpandSolution(successor->State, currHorizon + 1, dfs_depth, path_pr * successor->Prob);
          }
        }
      }
    }
    else{
      /* Assume successor states are sorted in order of probability */
      for(successor = node->BestAction->NextState; successor != NULL; successor = successor->Next){
        /* Stop if already visited this iteration or goal state */
        if(successor->State->Expanded < Iteration){
          /* If already expanded, just mark as visited this iteration */
          if(successor->State->Expanded > 0)
            successor->State->Expanded = Iteration;

          ExpandSolution(successor->State, currHorizon+1, dfs_depth, path_pr*successor->Prob);
        }
      }
    }
  }  
  else{
    cout << "not expanded, node: " << node->StateNo << endl;
    
    node->h = DBL_MAX;
    node->g = DBL_MAX;
    node->f = DBL_MAX;

    return;
  }

  if(gBackupMethod == combine)
    Backup(node, currHorizon);
}

void LAOStar::ExpandNode(struct StateNode *node, int currHorizon){
  list<StateNode*> tmpList1;

  if(DO_INCREMENTAL_HEURISTIC && node->hIncrement < MAX_H_INCREMENT){
    increment_heuristic(node);
    return;
  }

  if(node->Expanded == -1){
    for(ActionNodeList::iterator acts = node->NextActions->begin(); acts != node->NextActions->end(); acts++){
      for(StateDistribution* successor = (*acts)->NextState; successor != NULL; successor = successor->Next){
        if(successor->State->Expanded == -1){
          tmpList1.push_back(successor->State);
          getHeuristic(&tmpList1, node, 0);
          tmpList1.clear();
        }
      }
    }
  }

  if(max_horizon != -1 && currHorizon > max_horizon)
    return; 

  if(node->NextActions == NULL || node->Expanded == -1){
    node->horizon = currHorizon;
    node->expand();
  }

  node->Expanded = Iteration;

  /* set heuristic value */
  if(gBackupMethod == separate)
    ExpandedStateList->push_back(node);
  NumExpandedStates++;
}

StateList* LAOStar::findAncestors(StateList *ExpandedStateList){
  StateList AddList, NewAddList;
  StateList *AncestorList;
  StateList::iterator node;
  ActionNodeList::iterator prev;

  AncestorList = new StateList;

  /* Initial AddList consists of all expanded states */
  for(node = ExpandedStateList->begin(); node != ExpandedStateList->end(); node++){
    (*node)->Update = Iteration;
    AddList.push_back(*node);
  }

  /* Find ancestor states that need to be updated */
  while(!AddList.empty()){  /* check for empty list */
    NewAddList.clear();
    /* For each state added to Z ... */
    for(node = AddList.begin(); node != AddList.end(); node++){
      /* ... and for each parent of that state ... */
      for(prev = (*node)->PrevActions->begin(); prev != (*node)->PrevActions->end(); prev++){
        ActionNode* prevAct = *prev;
        /* only add a parent along a marked action arc */
        /* also, parent must be expanded */
        if(prevAct == prevAct->PrevState->BestAction && prevAct->PrevState->Expanded > 0){
          /* don't add parent that is already in ancestor list */
          if(prevAct->PrevState->Update < Iteration){
            NewAddList.push_back(prevAct->PrevState);
            prevAct->PrevState->Update = Iteration;
          }
        }
      }

      AncestorList->push_back(*node);
      NumAncestorStatesIter++;
    }

    AddList.swap(NewAddList);
  }

  return AncestorList;
}

void LAOStar::DisplaySolution(){
  NumSolutionStates = 0;
  Iteration++;
  DisplaySolutionRecursive(Start);
}

/* Set NumSolutionStates to zero before calling this recursive function */
StateList* LAOStar::DisplaySolutionRecursive(struct StateNode *node){
  struct StateDistribution *successor;

  printf("\nindex %d State f %f, g %f, h %f\n",
     node->StateNo,
     node->f,
     node->g,
     node->h
     );

  printf("GoalSatisfaction = %f\n", node->goalSatisfaction);
  NumSolutionStates++;
  if(node->Terminal == 0 && node->BestAction != NULL){
    printf("Best Act = \n");
    printAction(node->BestAction);

    for(successor = node->BestAction->NextState; successor != NULL; successor = successor->Next)
      printf("Next state = %d, Prob = %f\n",  successor->State->StateNo, successor->Prob);

    for(successor = node->BestAction->NextState; successor != NULL; successor = successor->Next){
      if(successor->State->Update < Iteration){
        successor->State->Update = Iteration;
        DisplaySolutionRecursive(successor->State);
      }
    }
  }
  else
    printf("<terminal>\n");
}

int LAOStar::ConvergenceTest(){
/* From start state, perform depth-first search of all states 
   visited by best solution. Mark each state when it is visited
   so that it is visited only once each pass. For each state:
   -- perform backup
   -- update Bellman residual
   If visit unexpanded state, exit with FALSE.
   If error bound <= epsilon, exit with TRUE.
   Otherwise repeat.
*/

  double error;

  ExitFlag = 0;
  do{
    Iteration++;
    NumSolutionStates = 0;
    Residual = 0.0;
    ConvergenceTestRecursive(Start, 0, 1.0);
    if(gDiscount < 1.0)
      error = (gDiscount * Residual) / (1.0 - gDiscount);
    else
      error = Start->meanFirstPassage * Residual;
  }
  while(!ExitFlag && error > gEpsilon);

  if(Start->Solved){
    cout << "SOLVED" << endl;
    return 1;
  }

  if(ExitFlag || error > gEpsilon)
    return 0;
  else
    return 1;
}

int LAOStar::ConvergenceTestRecursive( struct StateNode *node, int currHorizon, double path_pr){
  int oldAct, newAct;
  struct StateDistribution *successor;
  double Diff;

  /* Count this node as part of best solution */
  NumSolutionStates++; 

  /* Terminate recursion at goal node */
  if(node->Terminal == 1){
    node->prReached = path_pr;
    terminals.push_back(node);
    return TRUE;
  }

  /* Exit convergence test if best solution has unexpanded node */
  if(!node->Expanded){
    ExitFlag = 1;
    return FALSE;
  }

  if(node->BestAction == NULL){
    ExitFlag = 1;
    return TRUE;
  }

  /* Recursively consider unvisited successor nodes */
  for(successor = node->BestAction->NextState; successor != NULL; successor = successor->Next){
    if(successor->State->Update != Iteration){
      successor->State->Update = Iteration;
      ConvergenceTestRecursive(successor->State, 
                   currHorizon+1,
                   path_pr*successor->Prob);
    }
  }

  /* Do backup to improve value of node */
  oldAct = node->BestAction->ActionNo;

  Diff = Backup(node, currHorizon);
  if(Diff > Residual)
    Residual = Diff;

  newAct = node->BestAction->ActionNo;

  if(newAct != oldAct)
     ExitFlag = 1;

  return 0;
}