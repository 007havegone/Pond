#ifndef ASTAR_ADVANCE_H
#define ASTAR_ADVANCE_H

#include "graph.h"									// StateComparator
#include <set>
#include <deque>

class AStarAdvance{
protected:
  AStarAdvance(bool determinized = false);

  StateNode* advance(StateNode* start);
  StateNode* advance(std::set<StateNode*>& states);

  void updateSuccessors(StateNode* start, double delta_g, double oldPrReached, double newPrReached);

  void setBestAct(StateNode* state);

	std::set<StateNode*, StateComparator> open;

  bool determinized;

  int iteration;
};

#endif  // ASTAR_ADVANCE
