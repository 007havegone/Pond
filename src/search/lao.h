#ifndef LAO_H
#define LAO_H

#include "search.h"
#include "graph.h"

#include <list>

class LAOStar : public Search{
public:
  LAOStar();

  virtual void search();
  
protected:
  void uncheckAncestors(struct StateNode *node);
  void DisplaySolution();

  void ExpandSolution(struct StateNode *node, int currHorizon, int dfs_depth, double path_pr);
  void ExpandNode(struct StateNode *node, int);
  StateList *findAncestors(StateList *ExpandedStateList);
  int ConvergenceTest();
  int ConvergenceTestRecursive(struct StateNode *node, int currHorizon, double path_pr);
  StateList *DisplaySolutionRecursive(struct StateNode *node);

  int NumExpandedStates;
  int Iteration;
  int NumExpandedStatesIter;    /* number of states expanded this iteration */
  int NumAncestorStatesIter;
  int NumSolutionStates;
  StateList *ExpandedStateList;
  int CountBackups;
  double Residual;
  int ExitFlag;
  int converged_to_goal;

  int dfs_limit;

  std::list<StateNode*> terminals;

  StateNode* lastBackup;
};

#endif
