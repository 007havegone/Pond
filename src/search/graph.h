#ifndef GRAPH_H
#define GRAPH_H

#define PPDDL_PARSER 1                // Copied from globals.h ... FIX ME!

#include <ext/hash_map>
#include <map>
#include <list>
#include <set>
#include <vector>

#include "./movalfn.h"
#include "effects.h"

/* forward declaration of structures */
class RelaxedPlanLite;
class RelaxedPlan;
class StateNode;
class ActionNode;
class PolicyState;
struct StateDistribution;
struct DdNode;
typedef unsigned int BitVector;

struct eqstr
{
  bool operator()(const DdNode* p, const DdNode* q) const
  {
    return  p == q;
  }
};

#ifndef PPDDL_PARSER
struct alt_action;
#endif

typedef std::list<StateNode*> StateList;
typedef std::list<ActionNode*> ActionNodeList;

struct goal;

/* declaration of structures */
class StateNode {
public:
  StateNode();
  ~StateNode();

	// For sampling
	void addAllActions();
	ActionNode* getAction(const std::string name, bool addIfNeeded = true);

  void expand();

  void getAncestery(std::vector<StateNode*>& ancestery);
  static void getAncestery(std::vector<StateNode*>& states, std::vector<StateNode*>& ancestery);

  double forwardUpdate();
  double backwardUpdate(bool setBestAct = false);
  double valueUpdate(bool setBestAct = false);

  void valueIteration();
  static void valueIteration(std::vector<StateNode*>& states);

  static __gnu_cxx::hash_map<DdNode*, StateNode*, __gnu_cxx::hash<DdNode*>, eqstr> generated;

  void terminateLeaves();
  static void terminateLeaves(StateNode* start);

  static int                expandedStates;

  DdNode*                   dd;
  DdNode*                   backup_dd;
  DdNode*                   rp_states;
  int                       StateNo;
  char*                     Description;
  double                    f;
  double                    fWeight;
  double                    g;
  double                    h;
  double                    expH;
  double                    expDiscRew;
  double                    ExtendedGoalSatisfaction;
  double                    meanFirstPassage;   // upper bound on steps to goal
  double                    goalSatisfaction;
  double                    prReached;
  double                    bestActF;
  int                       num_supporters;
  int                       Terminal;
  int                       Expanded;
  int                       Update;
  int                       Backups;
  int                       Solved;             // 0 - not solved, 1 - fully solved, 2 - partially solved
  BitVector*                ha;
  int                       horizon;
  RelaxedPlanLite*          rpIncrement;
  int                       hIncrement;
  RelaxedPlan*              currentRP;
  DdNode*                   usedSamples;        // samples used from SAG by increments
  int                       numHvalues;
  double*                   hValues;
  int                       goalSamples;
  double                    kld;
  MOValueFunction*          moValueFunction;
  int												randPriority;

  ActionNode*               TerminalAction;
  ActionNode*               BestAction;
  ActionNodeList*           NextActions;
  ActionNodeList*           PrevActions;
  ActionNode*               BestPrevAction;

  int processSuccessors(std::list<StateNode*>* states, std::list<StateNode*>* fh_states);
  bool isGoal();

protected:
	// For sampling
	ActionNode* addAction(const std::string name, bool reuseIfExists = true, bool displayState = true);

  void applyDiscountedGoalProbability();
};

class ActionNode{
public:
  ActionNode();
  ~ActionNode();

	// For sampling
	StateNode* getState(const std::set<std::string>& trueObs, bool addIfNeeded = true);
	StateNode* newSample();

  static int                actionCount;

#ifdef PPDDL_PARSER
  const struct Action*      act;
#else
  struct alt_action*        act;
#endif 
  int                       ActionNo;
  double                    Cost;               // Immediate cost
  double                    expDiscRew;
  int                       Solved;             // 0 - not solved, 1 - fully solved, 2 - partially solved
  int                       penalty;
  struct StateDistribution* NextState;
  struct StateNode*         PrevState;
  struct StateDistribution* Determinization;    // The one state that this action is determinized for (ignore StateDistribution::Next)

protected:
	// For sampling
	StateNode* addState(const std::set<std::string>& trueObs, bool reuseIfExists = true);
	StateNode* newSample(std::list<std::map<const pEffect*, DdNode*>*>* observations);
};

class StateDistribution{
public:
  struct StateNode*         State;
  DdNode*                   reason;
  std::set<const pEffect*>*         creason;
  double                    Prob;
  double                    Reward;
  struct StateDistribution* Next;
};

class StateComparator{
public:
enum CompareMode{
  F_VAL,                  // F value
  PR_REACH,               // Probability of being reached
  EXP_EGS,                // Expected extended goal satisfaction
  HEUR                    // Heuristic value
};

  StateComparator();
  void init(CompareMode mode);

  bool operator() (StateNode *lhs, StateNode *rhs) const;
  bool operator() (PolicyState *lhs_ps, PolicyState *rhs_ps) const;

  CompareMode mode;
};

/* forward declaration of StateList functions */
extern void DisplayStateList(StateList *list);

/* external declaration of global data structures */
extern struct StateNode*    Start;
extern struct StateNode*    Goal;

#endif
