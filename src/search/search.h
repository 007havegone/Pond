#ifndef SEARCH_H
#define SEARCH_H

#include <list>

enum SearchType{
	ONLINE,
  ASTAR,
  EHC_SEARCH,
  MOLAO,
  LAO,
  RTDP_SEARCH,
  MOASTAR,
  REPLAN,
  HOP_SEARCH,
  PCHOP_SEARCH,
  ALAO_STAR,
  EXPH
};

class DdNode;
class StateNode;

class Search{
public:
  Search(SearchType type);
  virtual ~Search();

  virtual void init(int num_acts, DdNode* b_initial_state, DdNode*  b_goal_state);
	void resetPolicy();

	void incremental_search();
  virtual void search() = 0;

  SearchType getType(){ return type; }

protected:
  void printStateList();
  void graphToFile(const char* filename);

//private:
public:
  static SearchType type;     // TO-DO: Make it non-static once everything's in order
};

#endif
