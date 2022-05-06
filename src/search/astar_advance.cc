#include "astar_advance.h"

#include "globals.h"            // PointerHasher
#include "graph.h"              // StateNode, StateComparator
#include "search.h"
#include <set>
#include <deque>
#include <ext/hash_set>

using namespace __gnu_cxx;
using namespace std;

extern double gWeight;
extern void printBDD(DdNode*);

AStarAdvance::AStarAdvance(bool determinized){
	this->determinized = determinized;

	iteration = 0;
}

StateNode* AStarAdvance::advance(StateNode* start){
	set<StateNode*> states;
	states.insert(start);
	return advance(states);
}

StateNode* AStarAdvance::advance(set<StateNode*>& states){
	iteration++;

  open.clear();
	open.key_comp().init(StateComparator::F_VAL);

	StateNode *front = NULL;

	for(set<StateNode*>::iterator state_it = states.begin(); state_it != states.end(); state_it++){
		front = *state_it;
		assert(front->Solved <= 0);
		front->BestPrevAction = NULL;
		open.insert(front);
	}

	while(!open.empty()){
	  //	  printStateList();
		front = *open.begin();

		//printBDD(front->dd);

		if(front->isGoal() || front->Solved > 0){
			if(verbosity >= 1)
				cout << "\t" << front->StateNo << "\t" << front->g << "\t" << front->h << "\t"
					<< front->goalSatisfaction << "\t" << front->prReached << endl;
			return front;
		}

		open.erase(open.begin());
		if(front->Terminal > 0){
			front->Solved = 0;
			front->BestAction = NULL;
			continue;
		}

		if(front->Expanded <= 0){
			front->expand();
			front->BestAction = NULL;
		}
		front->Expanded = iteration;

		for(ActionNodeList::iterator act_it = front->NextActions->begin(); act_it != front->NextActions->end(); act_it++){
			ActionNode* action = *act_it;

			double new_g = front->g + action->Cost;

			double p = (double)rand() / (double)RAND_MAX;
			double cum_p = 0.0;

			for(StateDistribution* dist = action->NextState; dist != NULL; dist = dist->Next){
				StateNode* successor = dist->State;
// cout << dist->Prob << endl;
				if(dist->Prob <= 0.0)
					continue;

				if(determinized){
					cum_p += dist->Prob;
					if(cum_p <= p)
						continue;
				}

				set<StateNode *, StateComparator>::iterator successor_it = open.find(successor);
				bool inFringe = (successor_it != open.end());
				bool closed = (successor->Expanded == iteration);
				bool cheaper = (new_g < successor->g);

				if((!closed && (!inFringe || (inFringe && cheaper))) || (closed && cheaper)){
					action->Determinization = dist;
					successor->BestPrevAction = action;

					if(inFringe)
						open.erase(successor_it);

					if(max_horizon <= 0 && successor->h <= 0 && successor->goalSatisfaction < search_goal_threshold)
						successor->h = IPP_MAX_PLAN;

					successor->g = new_g;
					successor->f = successor->g + gWeight * successor->h;

					double oldPrReached = successor->prReached;
					successor->prReached = front->prReached * dist->Prob;

					if(inFringe)
						open.insert(successor_it, successor);
					else if(!closed)
						open.insert(successor);

          if(closed)
            updateSuccessors(successor, new_g - successor->g, oldPrReached, successor->prReached);
				}

				if(determinized)
					break;
			}
		}
	}

	return NULL;
}

void AStarAdvance::updateSuccessors(StateNode* start, double delta_g, double oldPrReached, double newPrReached){
  hash_set<StateNode *, PointerHasher> closed_up;
  set<StateNode *> open_up;
  open_up.insert(start);
  StateNode *front = NULL;

  while(!open_up.empty()){
    front = *open_up.begin();
    open_up.erase(open_up.begin());
    closed_up.insert(front);

    for(ActionNodeList::iterator act_it = front->NextActions->begin(); act_it != front->NextActions->end(); act_it++){
      ActionNode* action = *act_it;

      for(StateDistribution* dist = action->NextState; dist != NULL; dist = dist->Next){
        StateNode* successor = dist->State;

        if((determinized && action->Determinization != dist) || closed_up.count(successor) > 0 || dist->Prob <= 0.0)
          continue;

        set<StateNode *, StateComparator>::iterator successor_it = open.find(successor);
        bool inFringe = (successor_it != open.end());
        bool closed = (successor->Expanded == iteration);

        if(closed || inFringe){
          if(inFringe)
            open.erase(successor_it);

          successor->g += delta_g;
          successor->f = successor->g + gWeight * successor->h;
          successor->prReached = (successor->prReached / oldPrReached) * newPrReached;

          if(inFringe)
            open.insert(successor_it, successor);
          else// if(closed)
            open_up.insert(successor);
        }
      }
    }
  }
}

void AStarAdvance::setBestAct(StateNode* state){
	assert(state != NULL && (state->isGoal() || state->Solved > 0));

  if(state->Solved <= 0){
		state->BestAction = NULL;
		state->Solved = 1;
	}

	ActionNode* prevAction = NULL;
	StateNode* prevState = NULL;

	while((prevAction = state->BestPrevAction) != NULL && (prevState = prevAction->PrevState) != NULL && prevState->Solved <= 0){
		prevState->BestAction = prevAction;
		prevState->Solved = 1;
		state = prevState;
	}
}
