#ifndef STEP_SEARCH_H
#define STEP_SEARCH_H

#include "search.h"

// Forward Declarations
class StateNode;
class ActionNode;

class StepSearch : public Search{
protected:
	StepSearch(SearchType inheriting_type);
public:
	virtual ~StepSearch();

	virtual void search();

	virtual void setup(StateNode* start);
	virtual bool step() = 0;
	virtual double getQuality() = 0;
	virtual void store();
	virtual void recall();
	// 应该表示某个状态完成了搜索
	virtual void commit();
	virtual void cleanup();

protected:
	StateNode* start;
	ActionNode* bestFirstAction;
};

#endif	// STEP_SEARCH_H
