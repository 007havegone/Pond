#include "aostar.h"

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


AOStar::AOStar()
: StepSearch(AOSTAR){
}

void AOStar::setup(StateNode *start){
    StepSearch::setup(start);
    closed.clear();
    open.clear();
    
    start->m_status = UNEXPANDED;
    start->PrevActions = NULL;
    start->BestPrevAction = NULL;
    start->NextActions = NULL;
    open.insert(start);


}


// 当前状态出发，传递状态
void AOStar::goal_propagate(StateNode* n1, StateNode* n2)
{
    // 当前状态出发，传递后继状态
    ActionNodeList::iterator act_it;
    for (act_it = n1->NextActions->begin(); act_it != n1->NextActions->end(); ++act_it)
    {
        ActionNode *action = *act_it;
        StateDistribution *nextStateWarp = action->NextState;
        StateNode *nextState;
        while (nextStateWarp != NULL)
        {
            nextState = nextStateWarp->State;
            if(nextState != n2)
            {
                isolation_propagate(nextState);
                // 将Action从两个Node之间断开，还需要释放内存
                nextState->PrevActions->remove(action);
            }
            nextStateWarp = nextStateWarp->Next;
        }
    }
    // TO.DO 
    //删除所有后继状态，添加n2

    for (act_it = n1->PrevActions->begin(); act_it != n1->PrevActions->end();++act_it)
    {
        ActionNode *action = *act_it;
        StateNode *another;
        if (action->act->hasObservation()) // sensor action
        {
            StateNode *preState = action->PrevState;// 获取前继状态
            ActionNodeList::iterator ite = preState->NextActions->begin();
            another = NULL;
            while (ite != preState->NextActions->end()) // 这里使用ActionNo即Action->id目前不确定唯一性
            {
                if(*ite == action)
                {
                    ite++;
                    continue;
                }
                if ((*ite)->ActionNo == action->ActionNo) //另外一个And
                {
                    if((*ite)->NextState && (*ite)->NextState->State)
                    {
                        another = (*ite)->NextState->State;
                    }
                    break;
                }
                ite++;
            }
            assert(another != NULL);
            if(another->m_status == GOAL_REACHABLE){
                // 副结点也可达
                preState->m_status = GOAL_REACHABLE;
                goal_propagate(preState, n1, another);
            }
        }
        else
        {
            action->PrevState->m_status = GOAL_REACHABLE;
            goal_propagate(action->PrevState, n1);
        }
    }
}

void AOStar::goal_propagate(StateNode *par, StateNode* n1, StateNode* n2)
{
    // 当前状态出发，传递后继状态
    ActionNodeList::iterator act_it;
    for (act_it = par->NextActions->begin(); act_it != par->NextActions->end(); ++act_it)
    {
        ActionNode *action = *act_it;
        StateDistribution *nextStateWarp = action->NextState;
        StateNode *nextState;
        while (nextStateWarp != NULL)
        {
            nextState = nextStateWarp->State;
            if(nextState != n1 && nextState != n2)// 不等于目前处理的and
            {
                isolation_propagate(nextState);
                // 将Action从两个Node之间断开，还需要释放内存
                nextState->PrevActions->remove(action);
            }
            nextStateWarp = nextStateWarp->Next;
        }
    }
    // TO.DO 
    //删除所有后继状态
    //添加后继StateNode n1 n2

    for (act_it = par->PrevActions->begin(); act_it != par->PrevActions->end();++act_it)
    {
        ActionNode *action = *act_it;
        StateNode *another;
        StateNode *preState = action->PrevState;// 获取前继状态
        if (action->act->hasObservation()) // sensor action
        {
            ActionNodeList::iterator ite = preState->NextActions->begin();
            another = NULL;
            while (ite != preState->NextActions->end()) // 这里使用ActionNo即Action->id目前不确定唯一性
            {
                if(*ite == action)
                {
                    ite++;
                    continue;
                }
                if((*ite)->ActionNo == action->ActionNo)//另外一个And
                {
                    if((*ite)->NextState && (*ite)->NextState->State)
                    {
                        another = (*ite)->NextState->State;
                    }
                    break;
                }
                ite++;
            }
            assert(another != NULL);
            if(another->m_status == GOAL_REACHABLE){
                // 副结点也可达
                preState->m_status = GOAL_REACHABLE;
                goal_propagate(preState, n1, another);
            }
        }else
        {
            preState->m_status = GOAL_REACHABLE;
            goal_propagate(preState, n1);
        }
    }
}

void AOStar::dead_propagate(StateNode* node)
{
    while(node->PrevActions->size()>0)
    {
        ActionNode *action = node->PrevActions->front();
        StateNode *preState = action->PrevState;
        StateNode *another;
        if (action->act->hasObservation())
        {
            // 获取另一个And-State
            ActionNodeList::iterator ite = preState->NextActions->begin();
            another = NULL;
            while(ite != preState->NextActions->end())
            {
                if(*ite == action)
                {
                    ite++;
                    continue;
                }
                if((*ite)->ActionNo == action->ActionNo)
                {
                    if((*ite)->NextState && (*ite)->NextState->State)
                    {
                        another = (*ite)->NextState->State;
                    }
                    break;
                }
                ite++;
            }
            assert(another != NULL);
            isolation_propagate(another); // isolate another
            

        }
        else
        {

        }
    }
}
void AOStar::isolation_propagate(StateNode* node)
{
    node->m_active_in_transitions--;
    if(node->m_active_in_transitions || node->m_status == GOAL_REACHABLE)
        return;
    if(node->m_status == UNEXPANDED)
    {
        node->m_status == ISOLATED;
        return;
    }
    for (ActionNodeList::iterator ite = node->NextActions->begin(); ite != node->NextActions->end(); ++ite)
    {
        reconnection_propagate((*ite)->NextState->State);
    }
}

void AOStar::reconnection_propagate(StateNode* node)
{
    node->m_active_in_transitions++;
    if(node->m_active_in_transitions > 1 || node->m_status == GOAL_REACHABLE)
        return;
    if(node->m_status == ISOLATED)
        node->m_status == UNEXPANDED;
    else if(node->m_status == OUT_ISOLATED)
    {
        node->m_status = UNEXPANDED;
        open.insert(node);
    }
    else
    {
        for (ActionNodeList::iterator ite = node->NextActions->begin(); ite != node->NextActions->end();++ite)
        {
            reconnection_propagate((*ite)->NextState->State);
        }
    }
}
bool AOStar::new_action_transition(StateNode* node, ActionNode* action, DdNode* successor)
{
    bool isNew = StateNode::generated.count(successor) == 0;
    list<StateNode *> m_states;
    StateNode *child;
    if (isNew)
    {
        child = new StateNode();
        child->StateNo = state_count++;
        child->dd = successor;

        child->horizon = node->horizon + 1;
		child->kld = 1.0;
		child->goalSatisfaction = child->isGoal();// 更新该结点是否满足
		child->ExtendedGoalSatisfaction = child->goalSatisfaction;
        child->prReached = node->prReached * 1.0;
        child->g = node->g + 1;
        child->BestAction = NULL;
        child->horizon = node->horizon + 1;
        
        child->PrevActions = new ActionNodeList();
        child->NextActions = new ActionNodeList();
        action->NextState = new StateDistribution();

        action->NextState->State = child;
        action->NextState->Next = NULL;
        child->PrevActions->push_back(action);

        list<StateNode *> m_states;
        m_states.push_back(child);
        getHeuristic(&m_states, node, node->horizon + 1);
        m_states.clear();
        StateNode::generated[successor] = child;
        m_states.push_back(child);
        getHeuristic(&m_states, node, node->horizon + 1);
        // following from cnf planner
        if(child->isGoal()){// 当前满足goal
            child->m_status = GOAL_REACHABLE;
            node->m_status = GOAL_REACHABLE;
            goal_propagate( node, child);
        }
        else
        {
            open.insert(child);
        }
        StateNode::generated[successor] = child;
    }
    else
    {
        // 当前结点出现过
        child = StateNode::generated[successor];//直接获取stateNode
        // Dead-end
        if(child->dd == node->dd || child->m_status == DEAD)
        {
            return false;
        }
        if(child->m_status == GOAL_REACHABLE)
        {
            node->m_status = GOAL_REACHABLE;
            goal_propagate(node, child);
        }else
        {
            reconnection_propagate(child);
        }
    }
    // 前面该结点已经通过ActionNode连接两个StateNode
    // node -> child
    // child -> node

    return true;
}

bool AOStar::new_sensing_transition(StateNode* cur, const ActionNode* act, DdNode* c1, DdNode* c2)
{
    bool lExist = StateNode::generated.count(c1) != 0;
    bool rExist = StateNode::generated.count(c2) != 0;
    short lstatus = lExist ? StateNode::generated[c1]->m_status : -1;
    short rstatus = rExist ? StateNode::generated[c2]->m_status : -1;
    if (lExist && lstatus == DEAD ||
        rExist && rstatus == DEAD)
    {
        // TO.DO. release memory of c1 and c2.
        return false;
    }

    if( lExist && (lstatus == ISOLATED || lstatus == OUT_ISOLATED) ||
        rExist && (lstatus == OUT_ISOLATED || rstatus == OUT_ISOLATED))
    {
        // TO.DO release memory of c1 and c1.
        std::cout << "ISOLATION, IGNORE SENSING";
        return false;
    }

    if(lExist == false)
    {
        // TO.DO
    }
    else
    {

    }

    if(rExist == false)
    {
        // TO.DO
    }
    return true;
}
StateNode* AOStar::first_unexpanded()
{
    StateNode *cs;
    while (open.size() && (*open.begin())->m_status != UNEXPANDED)
    {
        if((*open.begin())->m_status == ISOLATED)
            (*open.begin())->m_status = OUT_ISOLATED;
        open.erase(open.begin());
    }
    if(open.size()){
        cs = *open.begin();
        open.erase(open.begin());
    }
    return NULL;
}
// 还缺少状态更新的部分
bool AOStar::expand(StateNode* node)
{
    StateNode *n1, *n2;
    map<const Action *, DdNode *>::iterator ite;
    ActionNodeList::iterator act_ite;
    // get the noop and executable action list
    for (ite = action_preconds.begin(); ite != action_preconds.end(); ite++)
    {
        if((*ite).first->name().compare("noop_action") ==0 )
            continue;
        DdNode *preBdd = (ite)->second;// 获取前提条件
        if(bdd_isnot_one(manager, bdd_imply(manager, node->dd, preBdd)))
        {
            continue;
        }
        ActionNode *actNode = new ActionNode();
        actNode->act = (*ite).first;
        actNode->PrevState = node;
        actNode->ActionNo = (*ite).first->id();
        node->NextActions->push_back(actNode);
    }

    for (act_ite = node->NextActions->begin(); act_ite != node->NextActions->end(); act_ite++)
    {
        ActionNode *action = *act_ite;
        DdNode *preBdd = action_preconds.find(action->act)->second;
        
        if(action->act->hasObservation())//sensor action
        {
            // TO.DO. Split get two successors
            // split()
            DdNode *s1;
            DdNode *s2;
            new_sensing_transition(node, action, s1, s2);

            if(Start->m_status == GOAL_REACHABLE){
                return true;
            }
            if(node->m_status == GOAL_REACHABLE){
                return false;
            }
        }
        else
        {
            pair<const Action *const, DdNode *> act_pair(action->act, preBdd);
            DdNode *successor = progress(&act_pair, node->dd);//根据当前状态和动作计算后继状态
            // 前面已经完成了Precondition测试
            if(successor == node->dd)
                continue;
            new_action_transition(node, action, successor);

            if(Start->m_status == GOAL_REACHABLE){
                return true;
            }
            if(node->m_status == GOAL_REACHABLE){
                return false;
            }
        }// end-And Node
    }
    if(node->NextActions->size()==0)
    {
        node->m_status = DEAD;
        cout << "DEAD " << node->StateNo << endl;
        dead_propagate(node);
        if(Start->m_status = DEAD)
            return true;
    }
    return false;
}
void AOStar::search()
{
    setup(Start);
    StateNode *node;

    if(Start->isGoal()){
        std::cout << "The initial state satisies the goal" << std::endl;
        return;
    }
    expandedNodes = 0;

    while (node = first_unexpanded())
    {
        expandedNodes++; // 拓展结点数+1
        if(expand(node))
        {
            break;
        }
    }
    if(Start->m_status != GOAL_REACHABLE){
        std::cout << "No plan was found.\n";
    }
    else{
        std::cout << "Solution was found\n";
    }
}