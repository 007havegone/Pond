#ifndef AOSTAR_H
#define AOSTAR_H

#include "search.h"
#include "graph.h"

#include <list>

class AOStar : public Search{
public:
   AOStar();
   virtual void search();

   protected:
   std::set<StateNode *> closed;
   std::set<StateNode *, StateComparator> open; // 通过启发式函数进行排序
   StateNode *next;								// 下一个处理结点
   bool first;
	
};
#endif
