#include "astar.h"

#include "globals.h"						// verbosity
#include "graph.h"              // StateNode
#include "../ppddl/actions.h"
#include <queue>
#include <set>
#include <cassert>

using namespace std;

extern double gWeight;

AStar::AStar()
: StepSearch(ASTAR){
}
/**
 * 算法初始化
 */
void AStar::setup(StateNode* start){
	StepSearch::setup(start);
	closed.clear();
	open.clear();

	next = start;
	next->BestPrevAction = NULL;//最佳action值为空
	open.insert(next);// 插入节点next

	first = true;//第一次调用
}
/**
 * 返回true，说明需要继续搜索
 * 返回false，说明查找失败或到达目标
 */
bool AStar::step(){
	assert(next != NULL);

	if(first){
		first = false;
		return true;
	}
	if(open.empty() || next->isGoal()){
		cout << "\t" << next->StateNo << "\t" << next->g << "\t" << next->h << "\t" << next->goalSatisfaction << "\t" << next->prReached << endl;
		if(open.empty())
			cout << "Dead end!\n";
		if(next->isGoal())
			cout << "Found branch!\n";
		return false;// 返回false停止搜索
	}

	open.erase(open.begin());
	closed.insert(next);
	// 添加所有满足当前状态动作到ActionNodeList中
	next->addAllActions();
	// 考虑每个动作结点
	for(ActionNodeList::iterator act_it = next->NextActions->begin(); act_it != next->NextActions->end(); act_it++){
		ActionNode* action = *act_it;
		StateNode* child = action->newSample();
		assert(child != NULL);

		double new_g = next->g + action->Cost;// A* Heuristic
		set<StateNode *, StateComparator>::iterator child_it = open.find(child);
		bool inFringe = (child_it != open.end());// 之前遇到过该状态
		bool isClosed = (closed.count(child) > 0);// 已经搜索过了
		bool cheaper = (new_g < child->g);// 是否比之前到达该child更便宜
		// 仅考虑没有被访问的情况
		// 之前没有加入open or 之前加入oepn但现在的路径更便宜，则需要更新
		if(!isClosed && (!inFringe || (inFringe && cheaper))){
			child->BestPrevAction = action;

			if(inFringe)// 删除open的节点
				open.erase(child_it);

			child->g = new_g;
			child->f = child->g + gWeight * child->h;

			child->prReached = next->prReached * 1.0;//dist->Prob;		// HACK
			child->horizon = next->horizon + 1;
			child->randPriority = rand();

			if(inFringe)
				open.insert(child_it, child);
			else
				open.insert(child);
		}
	}
	// 从open中取出最佳节点
	next = *open.begin();
	setBestAct(next);// 对next节点及前面

	return true;
}

double AStar::getQuality(){
	assert(next != NULL);
	return next->goalSatisfaction;	// Assuming conformant search
}

void AStar::commit(){
	StateNode* state = next;
	do{
		state->Solved = 1;
	} while(state->BestPrevAction != NULL && (state = state->BestPrevAction->PrevState) != NULL && state->Solved <= 0);
}
/**
 * 清空队列
 */
void AStar::cleanup(){
	StepSearch::cleanup();
	closed.clear();
	open.clear();
	next = NULL;
}

/**
 * momo007 011 2022.04.05 21.35
 * 这个函数在step中的作用。
 */
void AStar::setBestAct(StateNode* state){
	assert(state != NULL);
/*
  if(state->Solved <= 0)
		state->BestAction = NULL;
*/
	ActionNode* prevAction = NULL;
	StateNode* prevState = NULL;
	/**
	 * 从最后一个状态出发，设置每个状态的最佳动作
	 */
	while((prevAction = state->BestPrevAction) != NULL && (prevState = prevAction->PrevState) != NULL/* && prevState->Solved <= 0*/){
		prevState->BestAction = prevAction;
		state = prevState;
	}
}
