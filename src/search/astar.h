#ifndef ASTAR_H
#define ASTAR_H

#include "step_search.h"

#include "graph.h"									// StateComparator, StateNode, ActionNode
#include <set>
#include <deque>
#include <utility>									// pair

class AStar : public StepSearch{
public:
  AStar();

	void setup(StateNode* start);
	bool step();
	double getQuality();
	/**
	 * 更新当前节点及前驱节点都搜索完成，但没有被调用，应该是无关痛痒的函数
	 */
	void commit();
	void cleanup();

	void setBestAct(StateNode* state);

protected:
	std::set<StateNode*> closed;
	std::set<StateNode*, StateComparator> open;// 通过启发式函数进行排序
	StateNode* next;
	bool first;
};

#endif  // ASTAR_H
