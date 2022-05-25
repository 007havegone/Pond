#ifndef LAO_WRAPPER_H
#define LAO_WRAPPER_H

#include "graph.h"
#include <iostream>
#include <list>
#include <set>
#include <map>
#include <utility>
#include "dbn.h"
// 该文件应该封装了lao实现了 AO*算法
// 搜索类
class Search;

#ifdef PPDDL_PARSER
int not_loop(StateNode* dest, BitVector* visited, StateNode* srcNo);
void increment_heuristic(StateNode*);
/**
 * momo007 2022.05.25 not need in conformant
 */
// DdNode * apply_events(DdNode*);
DdNode* normalize(DdNode*);
double computeGoalSatisfaction(DdNode*);
struct StateFormula;
 DdNode* progress(std::pair<const Action* const, DdNode*>*, 
		 DdNode* parent);
//int split(std::map<const StateFormula*, DdNode*>,//std::pair<const Action* const, DdNode*>*, 
int split(std::list<DdNode*>*,
	  DdNode* parent, std::list<DdNode*>*, std::list<DdNode*>*);
int split(std::list<std::map<const pEffect*, DdNode* >*>* a,
	  DdNode* parent, std::list<DdNode*>* result,
	  std::list<std::set<const pEffect*>*>* reasons,
	  std::list<double>* probabilities,
	  int numSamples = 1
	  );
double computeReward(std::pair<const Action* const, DdNode*>* action, DdNode* successor, DdNode* parent);
#else
DdNode* progress(alt_action*, 
		 DdNode* parent);
int split(alt_action* a , DdNode* parent, std::list<DdNode*>*, std::list<DdNode*>*);

int validLifted(DdNode* state, StateNode* prevNode, alt_action *a);
DdNode* regress(alt_action*, DdNode*);
#endif

DdNode* progress(DdNode*image, DdNode* parent);
DdNode* progress(dbn*image, DdNode* parent);
void update_leaf_h_values();
void backup_graph();

struct action_list_node;
typedef action_list_node * action_list;
class goal_list;

void free_bit_vector(BitVector*);

struct StateDistribution *CreateStateDistribution( void );
void printAction(struct ActionNode* a);
void CreateHeuristic();
void DisplayStateList(StateList *list);
StateList *CreateStateList();  
BitVector *new_bit_vector(int length);
int get_bit(BitVector*, int, int);
DdNode** extractDNFfromBDD(DdNode* node);
DdNode** extractTermsFromMinterm(DdNode* node);
void set_bit(BitVector* b, int index);

int check_loop_conditions(StateNode* src, StateNode* dest, DdNode* state, int currHorizon);

DdNode * stateForward(DdNode*m,DdNode *f);
DdNode * stateBackward(DdNode*m,DdNode *f);

extern action_list available_acts;

void outputPlan();
void outputPlanR(StateNode* s, int indent, int, int&,int&, int&, BitVector*, double, std::list<double>*, double, std::list<double>*);

#endif

