/*
 * Copyright (C) 2004 Carnegie Mellon University
 * Written by H�kan L. S. Younes.
 *
 * Permission is hereby granted to distribute this software for
 * non-commercial research purposes, provided that this copyright
 * notice is included with any such distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE
 * SOFTWARE IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU
 * ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.
 *
 * $Id: mtbdd.cc,v 1.12 2007/04/10 18:53:52 dan Exp $
 */
#include "mtbdd.h"
#include "problems.h"
#include "domains.h"
#include "effects.h"
#include "formulas.h"
#include "functions.h"
#include <util.h>
#include <cudd.h>
#include <map>
#include <list>
#include <stdexcept>
#include <typeinfo>
#include "globals.h"
//#include "stl.h"
#include <stdlib.h>
#include <stdio.h>
#include "dd.h"
#include <exception>
#include <assert.h>
#include <math.h>

#include "dbn.h"
#include "lao_wrapper.h"
#include <float.h>
#include "solve.h"

extern int gnum_cond_effects;
/* Verbosity level. */
extern int verbosity;
extern void set_cubes();

DdNode* col_cube;
// /* The reward function. */
extern  Function reward_function;
// /* Whether the current domain defines a reward function. */
// static bool valid_reward_function;
// /* State variables for the current problem. */
// static std::map<const Atom*, int> state_variables;
// /* A mapping from variable indices to atoms. */
// static std::map<int, const Atom*> dynamic_atoms;
// /* DD manager. */
static DdManager* dd_man;
// /* Total number of state variables. */
static int nvars;
// /* BDDs representing identity between the `current state' and `next
//    state' versions of a variable. */
// static std::vector<DdNode*> identity_bdds;
// /* BDD representing identity between the `current state' and `next
//    state' versions of all state variables. */
// static DdNode* identity_bdd;
// /* MTBDDs representing transition probability matrices for actions. */
// static std::map<const Action*, DdNode*> action_transitions;
// /* MTBDDs representing reward vector for actions. */
// static std::map<const Action*, DdNode*> action_rewards;
// /* Mapping from action ids to actions used by current policy. */
static std::map<size_t, const Action*> policy_actions;

static __gnu_cxx::hash_set<const Atom*> init_variables;

/* ====================================================================== */
/* state_bdd */

/*
 * Returns a BDD representing the given state.
 */
static DdNode* state_bdd(DdManager* dd_man,
		const std::map<int, const Atom*>& dynamic_atoms,
		const AtomSet& atoms) {
	/* This is going to be the BDD representing the given state. */
	DdNode* dds = Cudd_ReadOne(dd_man);
	Cudd_Ref(dds);

	/*
	 * Set Boolean state variables to the values specified by the given
	 * atom set.
	 */
	for (std::map<int, const Atom*>::const_reverse_iterator ai =
			dynamic_atoms.rbegin();
			ai != dynamic_atoms.rend(); ai++) {
		int i = (*ai).first;
		DdNode* ddv = Cudd_bddIthVar(dd_man, 2*i);
		if (atoms.find((*ai).second) == atoms.end()) {
			ddv = Cudd_Not(ddv);
		}
		DdNode* ddt = Cudd_bddAnd(dd_man, ddv, dds);
		Cudd_Ref(ddt);
		Cudd_RecursiveDeref(dd_man, dds);
		dds = ddt;
	}

	return dds;
}


/* ====================================================================== */
/* collect_state_variables */
/*
 * Collects init state variables from the given formula.
 */
static void collect_init_state_variables(const StateFormula& formula) {

	// 是一个常数，结束
	if (typeid(formula) == typeid(Constant)) {
		/*
		 * The formula is either TRUE or FALSE, so it contains no state
		 * variables.
		 */
		return;
	}
	// 是一个atom
	const Atom* af = dynamic_cast<const Atom*>(&formula);
	if (af != NULL) {
		/*
		 * The formula is an atom representing a single state variable.
		 */
		if (init_variables.find(af) == init_variables.end()) {
			//std::cout << "got var" << std::endl;
			init_variables.insert(af);//第一次遇见加入
		}
		return;
	}
	// 后续根据是clause或者item或者one-of，forall均进行递归查找
	const Negation* nf = dynamic_cast<const Negation*>(&formula);
	if (nf != NULL) {
		/*
		 * The state variables of a negation are the state variables of the
		 * negand.
		 */
		collect_init_state_variables(nf->negand());
		return;
	}
	const Conjunction* cf = dynamic_cast<const Conjunction*>(&formula);
	if (cf != NULL) {
		/*
		 * The state variables of a conjunction are the state variables of
		 * the conjuncts.
		 */
		for (size_t i = 0; i < cf->size(); i++) {
			collect_init_state_variables(cf->conjunct(i));
		}
		return;
	}

	const Disjunction* df = dynamic_cast<const Disjunction*>(&formula);
	if (df != NULL) {
		/*
		 * The state variables of a disjunction are the state variables of
		 * the disjuncts.
		 */
		for (size_t i = 0; i < df->size(); i++) {
			collect_init_state_variables(df->disjunct(i));
		}
		return;
	}

	const OneOfDisjunction* odf = dynamic_cast<const OneOfDisjunction*>(&formula);
	if (odf != NULL) {
		/*
		 * The state variables of a disjunction are the state variables of
		 * the disjuncts.
		 */
		for (size_t i = 0; i < odf->size(); i++) {
			collect_init_state_variables(odf->disjunct(i));
		}
		return;
	}

	const Forall* faf = dynamic_cast<const Forall*>(&formula);
	if (faf != NULL) {
		/*
		 * The state variables of a disjunction are the state variables of
		 * the disjuncts.
		 */
		collect_init_state_variables(faf->body());
		return;
	}



	/*
	 * No other types of formulas should appear in fully instantiated
	 * action preconditions and effect conditions.
	 */
	throw std::logic_error("unexpected formula");
}


/*
 * Collects state variables from the given formula.
 */
static void collect_state_variables(const StateFormula& formula, bool initState, bool topLevel = true) {
	if (typeid(formula) == typeid(Constant)) {
		/*
		 * The formula is either TRUE or FALSE, so it contains no state
		 * variables.
		 */
		return;
	}

	const Atom* af = dynamic_cast<const Atom*>(&formula);
	if (af != NULL) {
		/*
		 * The formula is an atom representing a single state variable.
		 */
		if (state_variables.find(af) == state_variables.end()) {
			if((!topLevel && initState) ||  !initState){
				//	std::cout << "new var = " << dynamic_atoms.size() << " " << initState <<std::endl;
				dynamic_atoms.insert(std::make_pair(state_variables.size(), af));
			}
			state_variables.insert(std::make_pair(af, state_variables.size()));

		}
		return;
	}

	const Negation* nf = dynamic_cast<const Negation*>(&formula);
	if (nf != NULL) {
		/*
		 * The state variables of a negation are the state variables of the
		 * negand.
		 */
		if(topLevel)
			collect_state_variables(nf->negand(), initState, true);
		else
			collect_state_variables(nf->negand(), initState, false);
		return;
	}

	const Conjunction* cf = dynamic_cast<const Conjunction*>(&formula);
	if (cf != NULL) {
		/*
		 * The state variables of a conjunction are the state variables of
		 * the conjuncts.
		 */
		for (size_t i = 0; i < cf->size(); i++) {
			if(topLevel && initState)
				collect_state_variables(cf->conjunct(i), initState, true);
			else
				collect_state_variables(cf->conjunct(i), initState, false);
		}
		return;
	}

	const Disjunction* df = dynamic_cast<const Disjunction*>(&formula);
	if (df != NULL) {
		/*
		 * The state variables of a disjunction are the state variables of
		 * the disjuncts.
		 */
		for (size_t i = 0; i < df->size(); i++) {
			collect_state_variables(df->disjunct(i), initState, false);
		}
		return;
	}

	const OneOfDisjunction* odf = dynamic_cast<const OneOfDisjunction*>(&formula);
	if (odf != NULL) {
		/*
		 * The state variables of a disjunction are the state variables of
		 * the disjuncts.
		 */
		for (size_t i = 0; i < odf->size(); i++) {
			collect_state_variables(odf->disjunct(i), initState, false);
		}
		return;
	}
	const Forall* faf = dynamic_cast<const Forall*>(&formula);
	if (faf != NULL) {
		/*
		 * The state variables of a disjunction are the state variables of
		 * the disjuncts.
		 */
		collect_state_variables(faf->body(), initState, false);
		return;
	}
	/*
	 * No other types of formulas should appear in fully instantiated
	 * action preconditions and effect conditions.
	 */
	throw std::logic_error("unexpected formula");
}


/*
 * Collects state variables from the given effect.
 */
static void collect_state_variables(const pEffect& effect) {
	const AssignmentEffect* fe = dynamic_cast<const AssignmentEffect*>(&effect);
	if (fe != NULL) {
		/*
		 * Only reward assignments are supported, and they do not involve
		 * any state variables.
		 */
		if (fe->assignment().application().function() != reward_function) {
			throw std::logic_error("numeric state variables not supported");
		}
		return;
	}

	const SimpleEffect* se = dynamic_cast<const SimpleEffect*>(&effect);
	if (se != NULL) {
		/*
		 * A simple effect involves a single state variable.
		 */
		const Atom* atom = &se->atom();

		if (state_variables.find(atom) == state_variables.end()) {
			//std::cout << "AE " << dynamic_atoms.size() <<std::endl;
			dynamic_atoms.insert(std::make_pair(state_variables.size(), atom));
			state_variables.insert(std::make_pair(atom, state_variables.size()));
		}
		return;
	}

	const ConjunctiveEffect* ce =
			dynamic_cast<const ConjunctiveEffect*>(&effect);
	if (ce != NULL) {
		/*
		 * The state variables of a conjunctive effect are the state
		 * variables of the conjuncts.
		 */
		// std::cout << "CE" <<std::endl;
		for (size_t i = 0; i < ce->size(); i++) {
			collect_state_variables(ce->conjunct(i));
		}
		return;
	}

	const ConditionalEffect* we =
			dynamic_cast<const ConditionalEffect*>(&effect);
	if (we != NULL) {
		/*
		 * The state variables of a conditional effect are the state
		 * variables of the condition and the effect.
		 */
		// std::cout << "CndE" <<std::endl;
		collect_state_variables(we->condition(), false);
		collect_state_variables(we->effect());
		return;
	}

	const ProbabilisticEffect* pe =
			dynamic_cast<const ProbabilisticEffect*>(&effect);
	if (pe != NULL) {
		/*
		 * The state variables of a probabilistic effect are the state
		 * variables of the possible effects.
		 */
		//std::cout << "PrE" <<std::endl;
		for (size_t i = 0; i < pe->size(); i++) {
			collect_state_variables(pe->effect(i));
		}
		return;
	}

	/*
	 * No other types of effects exist.
	 */
	throw std::logic_error("unexpected effect");
}

/*
 * Checks if an effect is consistent
 */
static bool check_consistent(const pEffect& effect, 
		std::set<Atom*>* pos,
		std::set<Atom*>* neg) {
	const AssignmentEffect* fe = dynamic_cast<const AssignmentEffect*>(&effect);
	if (fe != NULL) {
		/*
		 * Only reward assignments are supported, and they do not involve
		 * any state variables.
		 */
		if (fe->assignment().application().function() != reward_function) {
			throw std::logic_error("numeric state variables not supported");
		}
		return false;
	}

	const AddEffect* adde = dynamic_cast<const AddEffect*>(&effect);
	if (adde != NULL) {
		Atom* atom = (Atom*) &adde->atom();
		pos->insert(atom);
		return true;
	}

	const DeleteEffect* deletee = dynamic_cast<const DeleteEffect*>(&effect);
	if (deletee != NULL) {
		Atom* atom = (Atom*) &deletee->atom();
		neg->insert(atom);
		return true;
	}

	const ConjunctiveEffect* ce =
			dynamic_cast<const ConjunctiveEffect*>(&effect);
	if (ce != NULL) {
		std::set<Atom*> mpos, mneg;
		mpos.insert(pos->begin(), pos->end());
		mneg.insert(neg->begin(), neg->end());
		for (size_t i = 0; i < ce->size(); i++) {
			if(!check_consistent(ce->conjunct(i), &mpos, &mneg))
				return false;

			std::set<Atom*> inter;
			std::insert_iterator<std::set<Atom*> > iter(inter, inter.begin());
			set_intersection(mpos.begin(), mpos.end(),
					mneg.begin(), mneg.end(),
					iter);
			if(inter.size() > 0)
				return false;

		}
		pos->insert(mpos.begin(), mpos.end());
		neg->insert(mneg.begin(), mneg.end());

		return true;
	}

	const ConditionalEffect* we =
			dynamic_cast<const ConditionalEffect*>(&effect);
	if (we != NULL) {
		return check_consistent(we->effect(), pos, neg);
	}

	const ProbabilisticEffect* pe =
			dynamic_cast<const ProbabilisticEffect*>(&effect);
	if (pe != NULL) {
		/*
		 * The state variables of a probabilistic effect are the state
		 * variables of the possible effects.
		 */
		for (size_t i = 0; i < pe->size(); i++) {
			check_consistent(pe->effect(i), pos, neg);
		}
		return true;
	}

	/*
	 * No other types of effects exist.
	 */
	throw std::logic_error("unexpected effect");
}

extern void printBDD(DdNode*);
/* ====================================================================== */
/* formula_bdd */

/*
 * Constructs a BDD representing the given formula.
 */
DdNode* formula_bdd(const StateFormula& formula, bool primed = false) {
	if (typeid(formula) == typeid(Constant)) {//判断是否常量
		/*
		 * The formula is either TRUE or FALSE, so the BDD is either
		 * constant 1 or 0.
		 */
		// cout << "const " << endl;
		DdNode* ddf = (formula.tautology() ?
				Cudd_ReadOne(dd_man) : Cudd_ReadLogicZero(dd_man));//here we use the logic zero
		Cudd_Ref(ddf);
		return ddf;
	}

	const Atom* af = dynamic_cast<const Atom*>(&formula);
	if (af != NULL) {
		/*
		 * The BDD for an atom is the `current-state' (or `next-state' if
		 * primed is true) DD variable for the state variable represented
		 * by the atom.
		 * primed代表状态变量加了'，即后即状态变量。
		 */
		//cout << "atom = " << state_variables[af] << endl;
		if(dynamic_atoms.find((state_variables[af])) == dynamic_atoms.end())
			return Cudd_ReadOne(manager);// 不是atom，返回1，为和？

		// 假设状态变量有[a,b,c,d]后即状态[a',b',c',d']排放位置为：[a,a',b,b',c,c',d,d']
		// 当前atom是状态变量，获取他的位置，然后状态BDD，并返回该Node
		DdNode* ddf = Cudd_bddIthVar(dd_man,
				2*state_variables[af] + (primed ? 1 : 0));
		Cudd_Ref(ddf);
		return ddf;
	}
	// 接下来分别考滤否定，合取，析取，one-of,forall的情况，递归处理一个formula

	const Negation* nf = dynamic_cast<const Negation*>(&formula);
	if (nf != NULL) {
		/*
		 * The BDD for a negation is the negation of the BDD for the
		 * negand.
		 *	递归对否定的内层处理，随后在对内层取否
		 */
		DdNode* ddn = formula_bdd(nf->negand(), primed);
		DdNode* ddf = Cudd_Not(ddn);
		Cudd_Ref(ddf);
		Cudd_RecursiveDeref(dd_man, ddn);
		return ddf;
	}
	
	const Conjunction* cf = dynamic_cast<const Conjunction*>(&formula);
	if (cf != NULL) {
		/*
		 * The BDD for a conjunction is the conjunction of the BDDs for
		 * the conjuncts.
		 * 这里readone为了和下面的conjunct合取
		 */
		DdNode* ddf = Cudd_ReadOne(dd_man);
		Cudd_Ref(ddf);

		for (size_t i = 0; i < cf->size(); i++) {
			DdNode* ddi = formula_bdd(cf->conjunct(i), primed);
			// momo007 012 2022.04.27
			// 这里确实应该不用加
			Cudd_Ref(ddi);//DON'T REALLY NEED THIS, except in uts k30.pddl
			DdNode* dda = Cudd_bddAnd(dd_man, ddf, ddi);
			Cudd_Ref(dda);
			Cudd_RecursiveDeref(dd_man, ddf);
			Cudd_RecursiveDeref(dd_man, ddi);
			ddf = dda;
		}
		return ddf;
	}
	
	const Disjunction* df = dynamic_cast<const Disjunction*>(&formula);
	if (df != NULL) {
		/*
		 * The BDD for a disjunction is the disjunction of the BDDs for
		 * the disjuncts.
		 * 类似前面的，这里clause所以是readzero随后析取
		 */
		DdNode* ddf = Cudd_ReadLogicZero(dd_man);
		Cudd_Ref(ddf);
		for (size_t i = 0; i < df->size(); i++) {
			DdNode* ddi = formula_bdd(df->disjunct(i), primed);
			DdNode* ddo = Cudd_bddOr(dd_man, ddf, ddi);
			Cudd_Ref(ddo);
			Cudd_RecursiveDeref(dd_man, ddf);
			Cudd_RecursiveDeref(dd_man, ddi);
			ddf = ddo;
		}
		return ddf;
	}

	const OneOfDisjunction* odf = dynamic_cast<const OneOfDisjunction*>(&formula);
	if (odf != NULL) {
		/*
		 * The BDD for a one of disjunction
		 */
		DdNode* ddx, *ddn;

		if(odf->size() == 2){
			ddx = formula_bdd(odf->disjunct(0), primed);
			Cudd_Ref(ddx);
			ddn = formula_bdd(odf->disjunct(1), primed);
			Cudd_Ref(ddn);
			// 上面两行加引用是为了下面的否定？
			if(ddx == Cudd_Not(ddn)){// 两者对立,oneof(a,!a)= (a or !a) and !(a and !a) = true
				Cudd_RecursiveDeref(manager, ddx);
				Cudd_RecursiveDeref(manager, ddn);
				ddx = Cudd_ReadOne(manager);
				Cudd_Ref(ddx);
				return ddx;
			}
			else{
				Cudd_RecursiveDeref(manager, ddx);
				Cudd_RecursiveDeref(manager, ddn);
			}
		}

		ddx = Cudd_ReadLogicZero(dd_man);
		Cudd_Ref(ddx);
		ddn = Cudd_ReadOne(dd_man);
		Cudd_Ref(ddn);
		// 考虑公式的每一个disjunct
		for (size_t i = 0; i < odf->size(); i++) {
			DdNode* ddi = formula_bdd(odf->disjunct(i), primed);
			DdNode* ddxp = Cudd_bddIte(dd_man, ddi, ddn, ddx);// (ddi and ddn) or (!ddi and ddx)
			Cudd_Ref(ddxp);

			DdNode *ddnp;
			if(i < odf->size()-1){
				ddnp = Cudd_bddIte(dd_man, ddi, Cudd_ReadLogicZero(manager), ddn);
				Cudd_Ref(ddnp);
			}
			Cudd_RecursiveDeref(dd_man, ddi);
			Cudd_RecursiveDeref(dd_man, ddx);
			Cudd_RecursiveDeref(dd_man, ddn);
			
			ddx = ddxp;
			Cudd_Ref(ddx);
			Cudd_RecursiveDeref(dd_man, ddxp);

			if(i < odf->size()-1){
				ddn = ddnp;
				Cudd_Ref(ddn);
				Cudd_RecursiveDeref(dd_man, ddnp);
			}


		}
		return ddx;
	}
	const Forall* faf = dynamic_cast<const Forall*>(&formula);
	// 对于forall，将其实例化，去除量词
	if (faf != NULL) {
		/*
		 * The state variables of a disjunction are the state variables of
		 * the disjuncts.
		 */
		const SubstitutionMap args;
		return formula_bdd(faf->instantiation(args, *my_problem));
	}

	/*
	 * No other types of formulae should appear in fully instantiated
	 * action preconditions and effect conditions.
	 */
	throw std::logic_error("unexpected formula");
}


void bdd_goal_cnf(std::list<DdNode*>* goal_cnf){
	DdNode *tmp;
	const Conjunction* cf = dynamic_cast<const Conjunction*>(&(my_problem->goal()));
	if (cf != NULL) {
		for (size_t i = 0; i < cf->size(); i++) {
			//cout << i << endl;
			const Atom* af1 = dynamic_cast<const Atom*>(&(cf->conjunct(i)));
			if(af1 != NULL){
				tmp = formula_bdd(*af1);
				Cudd_Ref(tmp);
				goal_cnf->push_back(tmp);
				continue;
			}
			const Negation* nf = dynamic_cast<const Negation*>(&(cf->conjunct(i)));
			if (nf != NULL) {
				tmp = formula_bdd(*nf);
				Cudd_Ref(tmp);
				goal_cnf->push_back(tmp);
				continue;
			}
			const Disjunction* df = dynamic_cast<const Disjunction*>(&(cf->conjunct(i)));
			if (df != NULL) {
				tmp = formula_bdd(*df);
				Cudd_Ref(tmp);
				goal_cnf->push_back(tmp);
				continue;
			}
			const OneOfDisjunction* odf = dynamic_cast<const OneOfDisjunction*>(&(cf->conjunct(i)));
			if (odf != NULL) {
				tmp = formula_bdd(*odf);
				Cudd_Ref(tmp);
				goal_cnf->push_back(tmp);
				continue;
			}
		}
	}
	else{
		tmp = formula_bdd((my_problem->goal()));
		Cudd_Ref(tmp);
		goal_cnf->push_back(tmp);
	}
}


/* ====================================================================== */
/* TransitionSet */

struct TransitionSetList;



/*
 * A list of transition sets.
 */


/* Fills the provided list with the conjunction of the given
   transition sets. */
void TransitionSet::conjunction(TransitionSetList& transitions,
		const TransitionSet& t1,
		const TransitionSet& t2) {
	/*
	 * Case 1: transitions in both sets.
	 */
	DdNode* dda = Cudd_bddAnd(dd_man, t1.condition_bdd(), t2.condition_bdd());
	Cudd_Ref(dda);
	if (dda != Cudd_ReadLogicZero(dd_man)) {
		DdNode* dde = Cudd_bddAnd(dd_man, t1.effect_bdd(), t2.effect_bdd());
		Cudd_Ref(dde);
		TransitionSet* t = new TransitionSet(dda, dde, t1.reward() + t2.reward(),
				t1.touched_variables());
		t->touched_variables().insert(t2.touched_variables().begin(),
				t2.touched_variables().end());
		transitions.push_back(t);
	} else {
		Cudd_RecursiveDeref(dd_man, dda);
	}
	/*
	 * Case 2: transitions only in the first set.  Ignore this case if
	 * the first transition set is a null transition set.
	 */
	if (!t1.is_null()) {
		DdNode* dd2 = Cudd_Not(t2.condition_bdd());
		Cudd_Ref(dd2);
		dda = Cudd_bddAnd(dd_man, t1.condition_bdd(), dd2);
		Cudd_Ref(dda);
		Cudd_RecursiveDeref(dd_man, dd2);
		if (dda != Cudd_ReadLogicZero(dd_man)) {
			DdNode* dde = t1.effect_bdd();
			Cudd_Ref(dde);
			TransitionSet* t = new TransitionSet(dda, dde, t1.reward(),
					t1.touched_variables());
			transitions.push_back(t);
		} else {
			Cudd_RecursiveDeref(dd_man, dda);
		}
	}
	/*
	 * Case 3: transition only in the second set.  Ignore this case if
	 * the second transition set is a null transition set.
	 */
	if (!t2.is_null()) {
		DdNode* dd1 = Cudd_Not(t1.condition_bdd());
		Cudd_Ref(dd1);
		dda = Cudd_bddAnd(dd_man, dd1, t2.condition_bdd());
		Cudd_Ref(dda);
		Cudd_RecursiveDeref(dd_man, dd1);
		if (dda != Cudd_ReadLogicZero(dd_man)) {
			DdNode* dde = t2.effect_bdd();
			Cudd_Ref(dde);
			TransitionSet* t = new TransitionSet(dda, dde, t2.reward(),
					t2.touched_variables());
			transitions.push_back(t);
		} else {
			Cudd_RecursiveDeref(dd_man, dda);
		}
	}
}


/* Constructs a null transition set. */
TransitionSet::TransitionSet()
: condition_bdd_(Cudd_ReadOne(dd_man)), reward_(0.0) {
	Cudd_Ref(condition_bdd_);
	effect_bdd_ = Cudd_ReadOne(dd_man);
	Cudd_Ref(effect_bdd_);
	set_index(gnum_cond_effects++);
}


/* Constructs a copy of the give transition set. */
TransitionSet::TransitionSet(const TransitionSet& t)
: condition_bdd_(t.condition_bdd_), effect_bdd_(t.effect_bdd_),
  reward_(t.reward_), touched_variables_(t.touched_variables_) {
	Cudd_Ref(condition_bdd_);
	Cudd_Ref(effect_bdd_);
	set_index(t.index());

}


/* Constructs a transition set for a simple effect. */
TransitionSet::TransitionSet(DdNode* condition_bdd,
		const Atom& atom, bool is_true)
: condition_bdd_(condition_bdd), reward_(0.0) {
	Cudd_Ref(condition_bdd_);
	int i = state_variables[&atom];
	DdNode* ddv = Cudd_bddIthVar(dd_man, 2*i + 1);
	Cudd_Ref(ddv);
	if (is_true) {
		effect_bdd_ = ddv;
	} else {
		effect_bdd_ = Cudd_Not(ddv);
		Cudd_Ref(effect_bdd_);
		Cudd_RecursiveDeref(dd_man, ddv);
	}
	touched_variables_.insert(i);
	set_index(gnum_cond_effects++);
}


/* Constructs a transition set for a reward effect. */
TransitionSet::TransitionSet(DdNode* condition_bdd, const Rational& reward)
: condition_bdd_(condition_bdd), reward_(reward) {
	Cudd_Ref(condition_bdd_);
	effect_bdd_ = Cudd_ReadOne(dd_man);
	Cudd_Ref(effect_bdd_);
	set_index(gnum_cond_effects++);
}


/* Deletes this transition set. */
TransitionSet::~TransitionSet() {
	Cudd_RecursiveDeref(dd_man, condition_bdd_);
	Cudd_RecursiveDeref(dd_man, effect_bdd_);
}


/* ====================================================================== */
/* OutcomeSet */

/*
 * Collection of probability weighted transition sets.
 */



/*
 * Deletes the given outcome set.
 */
static void free_outcomes(const OutcomeSet& outcomes) {
	for (std::vector<TransitionSetList>::const_iterator ei =
			outcomes.transitions.begin();
			ei != outcomes.transitions.end(); ei++) {
		for (TransitionSetList::const_iterator ti = (*ei).begin();
				ti != (*ei).end(); ti++) {
			delete *ti;
		}
	}
}


/*
 * Prints the given outcome set on the given stream.
 */
static void print_outcomes(std::ostream& os, const OutcomeSet& outcomes) {
	size_t n = outcomes.probabilities.size();
	for (size_t i = 0; i < n; i++) {
		os << "outcome " << i << ": " << outcomes.probabilities[i] << std::endl;
		for (TransitionSetList::const_iterator ti =
				outcomes.transitions[i].begin();
				ti != outcomes.transitions[i].end(); ti++) {
			os << "condition " << ti - outcomes.transitions[i].begin() << ':'
					<< std::endl;
			Cudd_PrintDebug(dd_man, (*ti)->condition_bdd(), 2*nvars, 2);
			os << "effect:" << std::endl;
			Cudd_PrintDebug(dd_man, (*ti)->effect_bdd(), 2*nvars, 2);
			os << "reward: " << (*ti)->reward() << std::endl;
			os << "touched variables:";
			for (std::set<int>::const_iterator vi =
					(*ti)->touched_variables().begin();
					vi != (*ti)->touched_variables().end(); vi++) {
				os << ' ' << *vi;
			}
			os << std::endl;
		}
	}
}


/*
 * Combines the given outcome sets, modifying the first set to make it
 * the combined outcome set.
 */
void combine_outcomes(OutcomeSet& outcomes1, const OutcomeSet& outcomes2) {
	size_t n = outcomes1.probabilities.size();
	size_t m = outcomes2.probabilities.size();
	//   cout << "n = " << n << " m = " << m <<endl;



	if(m == 0)
		return;

	for (size_t j = 1; j < m; j++) {
		const Rational& pj = outcomes2.probabilities[j];
		for (size_t i = 0; i < n; i++) {
			outcomes1.probabilities.push_back(outcomes1.probabilities[i]*pj);
			outcomes1.transitions.push_back(TransitionSetList());
			if (outcomes1.transitions[i].size() == 1
					&& outcomes1.transitions[i][0]->is_null()
					&& outcomes2.transitions[j].size() == 1
					&& outcomes2.transitions[j][0]->is_null()) {
				if (i + 1 == n) {
					outcomes1.transitions.back().push_back(outcomes2.transitions[j][0]);
				} else {
					outcomes1.transitions.back().push_back(new TransitionSet());
				}
			} else {
				for (TransitionSetList::const_iterator ti =
						outcomes1.transitions[i].begin();
						ti != outcomes1.transitions[i].end(); ti++) {
					if (!(*ti)->is_null()) {
						outcomes1.transitions.back().push_back(new TransitionSet(**ti));
					}
				}
				for (TransitionSetList::const_iterator ti =
						outcomes2.transitions[j].begin();
						ti != outcomes2.transitions[j].end(); ti++) {
					if (!(*ti)->is_null()) {
						if (i + 1 == n) {
							outcomes1.transitions.back().push_back(*ti);
						} else {
							outcomes1.transitions.back().push_back(new TransitionSet(**ti));
						}
					} else if (i + 1 == n) {
						delete *ti;
					}
				}
			}
		}
	}
	const Rational& p = outcomes2.probabilities[0];
	for (size_t i = 0; i < n; i++) {
		outcomes1.probabilities[i] = outcomes1.probabilities[i]*p;
		if (outcomes2.transitions[0].size() == 1
				&& outcomes2.transitions[0][0]->is_null()) {
			if (i + 1 == n) {
				delete outcomes2.transitions[0][0];
			}
		} else {
			if (outcomes1.transitions[i].size() == 1
					&& outcomes1.transitions[i][0]->is_null()) {
				delete outcomes1.transitions[i][0];
				outcomes1.transitions[i].clear();
			}
			if (i + 1 == n) {
				copy(outcomes2.transitions[0].begin(),
						outcomes2.transitions[0].end(),
						back_inserter(outcomes1.transitions[i]));
			} else {
				for (TransitionSetList::const_iterator ti =
						outcomes2.transitions[0].begin();
						ti != outcomes2.transitions[0].end(); ti++) {
					if (!(*ti)->is_null()) {
						outcomes1.transitions[i].push_back(new TransitionSet(**ti));
					}
				}
			}
		}
	}
}


/*
 * Collects outcomes for the given effect.
 */
static void effect_outcomes(OutcomeSet& outcomes,
		DdNode* condition_bdd, const pEffect& effect) {
	if (condition_bdd == Cudd_ReadLogicZero(dd_man)) {
		/*
		 * The condition is false, so no new outcomes are added.
		 */
		//    cout << "FALSE"<<endl;
		return;
	}

	const AssignmentEffect* fe = dynamic_cast<const AssignmentEffect*>(&effect);
	if (fe != NULL) {
		/*
		 * Only reward assignments are supported.  Add an outcome with
		 * probability 1 for the reward assigned by this effect.
		 */
		//      cout << "HI AE"<<endl;
		const Application& application = fe->assignment().application();
		if (application.function() != reward_function) {
			throw std::logic_error("numeric state variables not supported");
		}
		ValueMap values;
		values[&application] = 0.0;
		fe->assignment().affect(values);
		outcomes.probabilities.push_back(1);
		outcomes.transitions.push_back(TransitionSetList());
		const TransitionSet* t = new TransitionSet(condition_bdd,
				values[&application]);
		outcomes.transitions.back().push_back(t);
		//      cout << "BYE AE"<<endl;
		return;
	}

	const SimpleEffect* se = dynamic_cast<const SimpleEffect*>(&effect);
	if (se != NULL) {
		/*
		 * Add an outcome with probability 1 and a single transition for
		 * the simple effect.
		 */
		//    cout << "HI SE"<<endl;
		bool is_true = typeid(*se) == typeid(AddEffect);
		outcomes.probabilities.push_back(1);
		outcomes.transitions.push_back(TransitionSetList());
		const TransitionSet* t = new TransitionSet(condition_bdd,
				se->atom(), is_true);
		outcomes.transitions.back().push_back(t);
		//     cout << "BYE SE"<<endl;
		return;
	}

	const ConjunctiveEffect* ce =
			dynamic_cast<const ConjunctiveEffect*>(&effect);
	if (ce != NULL) {
		/*
		 * Collect the outcomes for the conjuncts.  All combinations of
		 * these outcomes are the outcomes for the conjunctive effect.
		 */
		//    cout << "HI CJE"<<endl;
		size_t n = ce->size();
		if (n > 0) {
			effect_outcomes(outcomes, condition_bdd, ce->conjunct(0));
			for (size_t i = 1; i < n; i++) {
				OutcomeSet c_outcomes;
				effect_outcomes(c_outcomes, condition_bdd, ce->conjunct(i));
				combine_outcomes(outcomes, c_outcomes);
				if(LUGTOTEXT){//since not calling ensure mutex, need to combine
					size_t n1 = outcomes.probabilities.size();
					for (size_t i = 0; i < n1; i++) {
						size_t m = outcomes.transitions[i].size();
						for (size_t j = 0; j < m - 1; j++) {
							for (size_t k = j + 1; k < m; k++) {
								const TransitionSet* t1 = outcomes.transitions[i][j];
								const TransitionSet* t2 = outcomes.transitions[i][k];
								if(t1->condition_bdd() == t2->condition_bdd()){
									DdNode* dde = Cudd_bddAnd(dd_man, t1->effect_bdd(),
											t2->effect_bdd());
									Cudd_Ref(dde);

									if(dde == Cudd_ReadLogicZero(manager))
										throw std::logic_error("inconsistent effect");

									DdNode *dda = t1->condition_bdd();
									Cudd_Ref(dda);
									TransitionSet* t = new TransitionSet(dda, dde,
											t1->reward() + t2->reward(),
											t1->touched_variables());
									t->touched_variables().insert(t2->touched_variables().begin(),
											t2->touched_variables().end());
									/* Replace transition set j with the new transition set. */
									outcomes.transitions[i][j] = t;
									/*
									 * Remove transition set k.
									 */
									outcomes.transitions[i][k] = outcomes.transitions[i].back();
									k--;
									outcomes.transitions[i].pop_back();
									m = outcomes.transitions[i].size();
									delete t1;
									delete t2;
								}
							}
						}


					}
				}
			}
		} else {
			outcomes.probabilities.push_back(1);
			outcomes.transitions.push_back(TransitionSetList());
			const TransitionSet* t = new TransitionSet(condition_bdd, 0.0);
			outcomes.transitions.back().push_back(t);
		}
		//      cout << "BYE CJE"<<endl;
		return;
	}

	const ConditionalEffect* we =
			dynamic_cast<const ConditionalEffect*>(&effect);
	if (we != NULL) {
		/*
		 * Take the conjunction of the condition f this effect and the
		 * condition inherited from nesting.  Make this the condition for
		 * the outcomes of the effect.
		 */
		//     cout << "HI CE"<<endl;
		DdNode* ddf = formula_bdd(we->condition());
		DdNode* ddc = Cudd_bddAnd(dd_man, condition_bdd, ddf);
		Cudd_Ref(ddc);
		Cudd_RecursiveDeref(dd_man, ddf);
		effect_outcomes(outcomes, ddc, we->effect());
		Cudd_RecursiveDeref(dd_man, ddc);
		//     cout << "BYE CE"<<endl;
		return;
	}

	const ProbabilisticEffect* pe =
			dynamic_cast<const ProbabilisticEffect*>(&effect);
	if (pe != NULL) {
		/*
		 * Add the outcomes of this probabilistic effect.
		 */
		//    cout << "HI PE"<<endl;
		Rational p_rest = 1;
		size_t n = pe->size();
		//cout << "n = " << n << " pr0 = " << (pe->probability(0).double_value()) <<endl;
		if(n == 1 && (pe->probability(0).double_value()) == -2.0) {//unknown effect
			Rational p(0.5);
			OutcomeSet p_outcomes;
			effect_outcomes(p_outcomes, condition_bdd, pe->effect(0));
			for (std::vector<Rational>::const_iterator pi =
					p_outcomes.probabilities.begin();
					pi != p_outcomes.probabilities.end(); pi++) {
				outcomes.probabilities.push_back(p*(*pi));
			}
			copy(p_outcomes.transitions.begin(), p_outcomes.transitions.end(),
					back_inserter(outcomes.transitions));

			Rational p1(0.5);
			OutcomeSet p1_outcomes;
			DeleteEffect p1e (((SimpleEffect&)pe->effect(0)).atom());
			effect_outcomes(p1_outcomes, condition_bdd, p1e);
			for (std::vector<Rational>::const_iterator pi =
					p1_outcomes.probabilities.begin();
					pi != p1_outcomes.probabilities.end(); pi++) {
				outcomes.probabilities.push_back(p1*(*pi));
			}
			copy(p1_outcomes.transitions.begin(), p1_outcomes.transitions.end(),
					back_inserter(outcomes.transitions));


		}
		else if(n > 1 && fabs(pe->probability(0).double_value()) == 1.0){ //oneof effect
			for (size_t i = 0; i < n; i++) {

				Rational p (1.0/(double)n);
				OutcomeSet p_outcomes;
				effect_outcomes(p_outcomes, condition_bdd, pe->effect(i));
				for (std::vector<Rational>::const_iterator pi =
						p_outcomes.probabilities.begin();
						pi != p_outcomes.probabilities.end(); pi++) {
					outcomes.probabilities.push_back(p*(*pi));
				}
				copy(p_outcomes.transitions.begin(), p_outcomes.transitions.end(),
						back_inserter(outcomes.transitions));
			}
		}
		else{ //probabilistic effect
			for (size_t i = 0; i < n; i++) {
				Rational p = pe->probability(i);
				//	cout << "i = " << i << " " << p  << endl;
				p_rest = p_rest - p;
				OutcomeSet p_outcomes;
				effect_outcomes(p_outcomes, condition_bdd, pe->effect(i));
				for (std::vector<Rational>::const_iterator pi =
						p_outcomes.probabilities.begin();
						pi != p_outcomes.probabilities.end(); pi++) {
					outcomes.probabilities.push_back(p*(*pi));
				}
				copy(p_outcomes.transitions.begin(), p_outcomes.transitions.end(),
						back_inserter(outcomes.transitions));
			}
			if (p_rest > 0.0 && pe->probability(0) > 0.0) {//only for prob effects
				/*
				 * Add an outcome with a null transition set, weighted by the
				 * remaining probability.
				 */
				outcomes.probabilities.push_back(p_rest);
				outcomes.transitions.push_back(TransitionSetList());
				outcomes.transitions.back().push_back(new TransitionSet());
			}
		}
		//    cout << "BYE PE"<<endl;
		return;
	}

	/*
	 * No other types of effects exist.
	 */
	throw std::logic_error("unexpected effect");
}


/*
 * Collects dbn for the given effect.
 */
static dbn* effect_dbn(const pEffect& effect, std::map<const pEffect*, dbn_node*> * pr_nodes) {



	const AssignmentEffect* fe = dynamic_cast<const AssignmentEffect*>(&effect);
	if (fe != NULL) {
		/*
		 * Only reward assignments are supported.  Add an outcome with
		 * probability 1 for the reward assigned by this effect.
		 */
		//           std::cout << "HI AE"<<std::endl;
		const Application& application = fe->assignment().application();
		if (application.function() != reward_function) {
			throw std::logic_error("numeric state variables not supported");
		}
		ValueMap values;
		values[&application] = 0.0;
		fe->assignment().affect(values);
		//    outcomes.probabilities.push_back(1);
		//    outcomes.transitions.push_back(TransitionSetList());
		//    const TransitionSet* t = new TransitionSet(condition_bdd,
		//					       values[&application]);
		//    outcomes.transitions.back().push_back(t);
		//std::cout << "BYE AE"<<std::endl;
		return dbn::reward(new dbn(num_alt_facts), &values,(*pr_nodes)[&effect]);//new dbn(num_alt_facts);
	}

	const SimpleEffect* se = dynamic_cast<const SimpleEffect*>(&effect);
	if (se != NULL) {
		/*
		 * Add an outcome with probability 1 and a single transition for
		 * the simple effect.
		 */
		//     std::cout << "HI SE"<<std::endl;
		bool is_true = typeid(*se) == typeid(AddEffect);
		//    outcomes.probabilities.push_back(1);
		//    outcomes.transitions.push_back(TransitionSetList());
		//    const TransitionSet* t = new TransitionSet(condition_bdd,
		//					       se->atom(), is_true);
		//    outcomes.transitions.back().push_back(t);


		dbn *mdbn = dbn::simple_effect(new dbn(num_alt_facts),
				state_variables[&(se->atom())],
				(is_true ? 1 : 0));
		//     std::cout << "se: " << state_variables[&(se->atom())] << " "
		// 	      << (is_true ? 1 : 0) << std::endl;

		//std::cout << *mdbn << std::endl;
		//std::cout << "BYE SE"<<std::endl;

		return mdbn;
	}

	const ConjunctiveEffect* ce =
			dynamic_cast<const ConjunctiveEffect*>(&effect);
	if (ce != NULL) {
		/*
		 * Collect the outcomes for the conjuncts.  All combinations of
		 * these outcomes are the outcomes for the conjunctive effect.
		 */
		//     std::cout << "HI CJE" << std::endl;
		size_t n = ce->size();
		std::list<dbn*> conjuncts;
		//    std::cout << n << " conjuncts"<<std::endl;
		if (n > 0) {
			for (size_t i = 0; i < n; i++) {
				dbn *d = effect_dbn(ce->conjunct(i), pr_nodes);
						
				if(d != NULL){
					conjuncts.push_back(d);
				}
			}
		}


		dbn* mdbn = dbn::conjoin_dbns(&conjuncts, NULL);
		for(std::list<dbn*>::iterator i = conjuncts.begin();
				i != conjuncts.end(); i++)
			delete *i;
		//std::cout << *mdbn << std::endl;
		//  std::cout << "BYE CJE"<<std::endl;
		//      std::cout << *mdbn << std::endl;
		return mdbn;
	}

	const ConditionalEffect* we =
			dynamic_cast<const ConditionalEffect*>(&effect);
	if (we != NULL) {
		/*
		 * Take the conjunction of the condition for this effect and the
		 * condition inherited from nesting.  Make this the condition for
		 * the outcomes of the effect.
		 */
		//        std::cout << "HI CE"<<std::endl;
		DdNode* ddf = formula_bdd(we->condition());
		Cudd_Ref(ddf);

		//printBDD(ddf);

		dbn *edbn = effect_dbn(we->effect(), pr_nodes);
		//std::cout << "[" << std::flush; Cudd_CheckKeys(manager);
		dbn *mdbn = dbn::condition_dbn(ddf, edbn);
		//std::cout << "|" << std::flush; Cudd_CheckKeys(manager); std::cout << "]" << std::endl;
		//Cudd_RecursiveDeref(dd_man, ddf);
		delete edbn;


		//std::cout << *mdbn << std::endl;
		//std::cout << "BYE CE"<<std::endl;
		//std::cout << *mdbn << std::endl;
		return mdbn;
	}

	const ProbabilisticEffect* pe =
			dynamic_cast<const ProbabilisticEffect*>(&effect);
	if (pe != NULL) {
		/*
		 * Add the outcomes of this probabilistic effect.
		 */
		//    std::cout << "HI PE"<<std::endl;
		Rational p_rest = 1;
		size_t n = pe->size();
		std::list<dbn*> conjuncts;
		std::list<double> prs;
		for (size_t i = 0; i < n; i++) {
			Rational p = pe->probability(i);
			if(p.double_value() == 0.0)
				continue;
			// std::cout << "i = " << i << " " << p  << std::endl;
			p_rest = p_rest - p;
			conjuncts.push_back(effect_dbn(pe->effect(i), pr_nodes));
			prs.push_back(p.double_value());
			if(p.double_value() == 1.0)
				break;
		}
		if(p_rest > 0.0){
			dbn* mydbn = new dbn(num_alt_facts);
			conjuncts.push_back(mydbn);
			prs.push_back(p_rest.double_value());
		}



		dbn *mdbn = (prs.size() > 1 ?
				dbn::probabilistic_dbns(&prs, &conjuncts, (*pr_nodes)[&effect]) :
				conjuncts.front());


		for(std::list<dbn*>::iterator i = conjuncts.begin();
				(prs.size() > 1 &&  i != conjuncts.end()); i++)
			delete *i;

		//   std::cout << *mdbn << std::endl;
		//  std::cout << "BYE PE"<< std::endl;
		return mdbn;
	}

	/*
	 * No other types of effects exist.
	 */
	throw std::logic_error("unexpected effect");
}

/* ====================================================================== */
/* ensure_mutex */

/*
 * Ensures that all transition set conditions are mutually exclusive
 * for each probabilistic outcome.
 */
static void ensure_mutex(OutcomeSet& outcomes) {
	size_t n = outcomes.probabilities.size();
	for (size_t i = 0; i < n; i++) {
		size_t m = outcomes.transitions[i].size();
		for (size_t j = 0; j < m - 1; j++) {
			for (size_t k = j + 1; k < m; k++) {
				const TransitionSet* t1 = outcomes.transitions[i][j];
				const TransitionSet* t2 = outcomes.transitions[i][k];
				DdNode* dda = Cudd_bddAnd(dd_man, t1->condition_bdd(),
						t2->condition_bdd());
				Cudd_Ref(dda);
				if (dda != Cudd_ReadLogicZero(dd_man)) {
					/*
					 * Transition sets j and k do not have mutually exclusive
					 * conditions, so split them into transition sets that have.
					 *
					 * Case 1: both transition set conditions are enabled.
					 */
					DdNode* dde = Cudd_bddAnd(dd_man, t1->effect_bdd(),
							t2->effect_bdd());
					Cudd_Ref(dde);
					TransitionSet* t = new TransitionSet(dda, dde,
							t1->reward() + t2->reward(),
							t1->touched_variables());
					t->touched_variables().insert(t2->touched_variables().begin(),
							t2->touched_variables().end());
					/* Replace transition set j with the new transition set. */
					outcomes.transitions[i][j] = t;

					/*
					 * Case 2: only condition for transition set j is enabled.
					 */
					DdNode* dd2 = Cudd_Not(t2->condition_bdd());
					Cudd_Ref(dd2);
					dda = Cudd_bddAnd(dd_man, t1->condition_bdd(), dd2);
					Cudd_Ref(dda);
					Cudd_RecursiveDeref(dd_man, dd2);
					if (dda != Cudd_ReadLogicZero(dd_man)) {
						dde = t1->effect_bdd();
						Cudd_Ref(dde);
						TransitionSet* t = new TransitionSet(dda, dde, t1->reward(),
								t1->touched_variables());
						/* Add new transition set. */
						outcomes.transitions[i].push_back(t);
					} else {
						Cudd_RecursiveDeref(dd_man, dda);
					}

					/*
					 * Case 3: only condition for transition set k is enabled.
					 */
					DdNode* dd1 = Cudd_Not(t1->condition_bdd());
					Cudd_Ref(dd1);
					dda = Cudd_bddAnd(dd_man, dd1, t2->condition_bdd());
					Cudd_Ref(dda);
					Cudd_RecursiveDeref(dd_man, dd1);
					if (dda != Cudd_ReadLogicZero(dd_man)) {
						dde = t2->effect_bdd();
						Cudd_Ref(dde);
						TransitionSet* t = new TransitionSet(dda, dde, t2->reward(),
								t2->touched_variables());
						/* Add new transition set. */
						outcomes.transitions[i].push_back(t);
					} else {
						Cudd_RecursiveDeref(dd_man, dda);
					}

					/*
					 * Remove transition set k.
					 */
					outcomes.transitions[i][k] = outcomes.transitions[i].back();
					k--;
					outcomes.transitions[i].pop_back();
					m = outcomes.transitions[i].size();
					delete t1;
					delete t2;
				} else {
					Cudd_RecursiveDeref(dd_man, dda);
				}
			}
		}
	}
}


/* ====================================================================== */
/* matrix_to_dbn */

/* Constructs a DBN representation of the given transition probability
   matrix. */
static void matrix_to_dbn(DdNode* ddP) {
	DdNode* ddp = ddP;
	Cudd_Ref(ddp);
	DdNode** cube_vars = new DdNode*[nvars];
	for (int i = 0; i < nvars; i++) {
		cube_vars[i] = Cudd_addIthVar(dd_man, 2*i + 1);
		Cudd_Ref(cube_vars[i]);
	}
	/*
	 * Construct a DBN from the joint probability distribution
	 * (represented by the full transition probability matrix).  We have
	 * chosen to process the variables in a fixed order, but a different
	 * ordering could very well produce a more compact representation.
	 *
	 * The DBN for an action will contain synchronic arcs if the action
	 * has correlated effects.
	 */
	for (int i = nvars - 1; i >= 0; i--) {
		/*
		 * Sum over `next-state' variables that have not yet been
		 * processes.  This ensures an acyclic structure.
		 */
		DdNode* ddc = Cudd_addComputeCube(dd_man, cube_vars, NULL, i);
		Cudd_Ref(ddc);
		DdNode* ddt = Cudd_addExistAbstract(dd_man, ddp, ddc);
		Cudd_Ref(ddt);
		Cudd_RecursiveDeref(dd_man, ddc);
		/*
		 * We are only interested in cases where the current `next-state'
		 * variable is true.  This is because we are trying to compute the
		 * probability that the current `next-state' variable is true
		 * conditioned on (possibly all) `current-state' variables and
		 * unprocesses `next-state' variables.
		 */
		DdNode* dds = Cudd_addApply(dd_man, Cudd_addTimes, cube_vars[i], ddt);
		Cudd_Ref(dds);
		/*
		 * The above step ensures that the current `next-state' variable
		 * is true, so we do not need to encode this in the MTBDD.
		 */
		ddc = Cudd_addComputeCube(dd_man, &cube_vars[i], NULL, 1);
		Cudd_Ref(ddc);
		DdNode* ddm = Cudd_addExistAbstract(dd_man, dds, ddc);
		Cudd_Ref(ddm);
		Cudd_RecursiveDeref(dd_man, dds);
		Cudd_RecursiveDeref(dd_man, ddc);
		/*
		 * At this point, ddm is an MTBDD representing the conditional
		 * probability table for the current `next-state' variable being
		 * true.
		 */
		std::cout << "CPT for variable " << i << std::endl;
		Cudd_PrintDebug(dd_man, ddm, 2*nvars, 2);
		Cudd_RecursiveDeref(dd_man, ddm);
		/*
		 * Factor the joint probability distribution before we continue
		 * with the next variable.
		 */
		dds = Cudd_addApply(dd_man, Cudd_addDivide, ddp, ddt);
		Cudd_Ref(dds);
		Cudd_RecursiveDeref(dd_man, ddp);
		Cudd_RecursiveDeref(dd_man, ddt);
		ddp = dds;
	}
	for (int i = 0; i < nvars; i++) {
		Cudd_RecursiveDeref(dd_man, cube_vars[i]);
	}
	Cudd_RecursiveDeref(dd_man, ddp);
}

extern void printBDD(DdNode*);

void recurse_compose_outcomes(std::map<DdNode*, std::pair<double, double>*>* observables, 
		std::list<DdNode*>* observations,
		DdNode* obs){
	if(observables->empty()){
		if(obs == Cudd_ReadLogicZero(manager))
			return;

		Cudd_Ref(obs);
		observations->push_back(obs);

	}
	else{
		std::map<DdNode*, std::pair<double, double>*>::iterator i = observables->begin();
		std::pair<DdNode*, std::pair<double, double>*> t = *i;
		observables->erase(i);

		DdNode *pos = Cudd_bddAnd(manager, obs, (*i).first);
		Cudd_Ref(pos);
		DdNode *neg = Cudd_bddAnd(manager, obs, Cudd_Not((*i).first));
		Cudd_Ref(neg);

		if(pos != Cudd_ReadLogicZero(manager))
			recurse_compose_outcomes(observables, observations, pos);
		if(neg != Cudd_ReadLogicZero(manager))
			recurse_compose_outcomes(observables, observations, neg);
		Cudd_RecursiveDeref(manager, pos);
		Cudd_RecursiveDeref(manager, neg);

		(*observables)[t.first] = t.second;
	}
}

struct effcomp{ bool operator()(const pEffect* e1, const pEffect* e2) const { return e1->operator<(*e2);}};

void recurse_compose_outcomes( std::set<std::map<const pEffect*, DdNode*, effcomp>*> * observables, 
		std::map<std::set<const pEffect*>*, DdNode*>* observations,
		DdNode* obs,
		std::set<const pEffect*>* atoms){
	if(observables->empty()){
		// if(obs == Cudd_ReadLogicZero(manager))
			//return;

		Cudd_Ref(obs);
		std::set<const pEffect*> *ac = new std::set<const pEffect*>();
		ac->insert(atoms->begin(), atoms->end());
		(*observations)[ac] = obs;

//		std::cout << "GOT OBS:" << std::endl;
//		printBDD(obs);
	}
	else{
		std::set<std::map<const pEffect*, DdNode*, effcomp>*>::iterator i = observables->begin();
		std::map<const pEffect*, DdNode*, effcomp>* t = *i;
		observables->erase(i);
		  //  std::cout << (*i)->size() << std::endl;
		int is = (*i)->size();
		for(std::map<const pEffect*, DdNode*,effcomp>::iterator j = (*i)->begin();
				j != (*i)->end(); j++){
//			std::cout << "hi" << std::endl;

				if(is == 0)
				break;
			is--;

			//if(!(*j).first || !(*j).second)
			//	continue;
			//     printBDD(obs);
			//printBDD((*j).second);
			DdNode *pos = Cudd_addApply(manager, Cudd_addTimes,  obs, (*j).second);
			Cudd_Ref(pos);

		//	std::cout << " pos " << std::endl;
//			printBDD(pos);

			atoms->insert((*j).first);
			//if(pos != Cudd_ReadLogicZero(manager))
			recurse_compose_outcomes(observables, observations, pos, atoms);
		    atoms->erase((*j).first);
			Cudd_RecursiveDeref(manager, pos);

		}
		  //  std::cout << "exit" << std::endl;
		observables->insert(t);
	}
}


/* ====================================================================== */
/* action_mtbdds */

//Pr(o1 | s) 1-Pr(-o1 | s)
//
//Pr(o1 | s) Pr(o1 | -s)

//Pr(o1,o2,o3 | s)
//
//Pr(o1,o2,o3 | s)

std::list<DdNode*>*
pomdp_observation_mtbdds(ObservationVector& ob,
		const Problem& problem){
	ObservationEntry *e;
	std::map<DdNode*, DdNode*> observables;
	std::list<DdNode*>* observations = new std::list<DdNode*>();
	DdNode *symbol, *states, *tmp, *pr;

	//  cout << "|vars| = " << state_variables.size() << endl;


	for(ObservationVector::iterator i = //(const struct ObservationEntry**)
			ob.begin();
			i != ob.end(); i++){

		e = ((ObservationEntry*)(*i));
		// cout << "symbol"<<endl;
		symbol = formula_bdd(e->symbol());
		Cudd_Ref(symbol);

		//     e->symbol().print(cout, problem.domain().predicates(),
		// 		      problem.domain().functions(),
		// 		      problem.terms());
		//printBDD(symbol);

		tmp = formula_bdd(e->formula());
		Cudd_Ref(tmp);



		states = Cudd_BddToAdd(manager, tmp);
		Cudd_Ref(states);
		Cudd_RecursiveDeref(manager, tmp);
		pr = Cudd_addConst(manager, e->posProbability().double_value());
		Cudd_Ref(pr);
		tmp = Cudd_addApply(manager, Cudd_addTimes, pr, states);
		Cudd_Ref(tmp);
		Cudd_RecursiveDeref(manager, pr);
		Cudd_RecursiveDeref(manager, states);

		if(observables.find(symbol) == observables.end())
			states = Cudd_ReadZero(manager);
		else
			states = observables[symbol];

		Cudd_Ref(states);

		pr = Cudd_addApply(manager, Cudd_addPlus, states, tmp);

		Cudd_Ref(pr);

		observables[symbol] = pr;

		//printBDD(pr);

		Cudd_RecursiveDeref(manager, states);
		Cudd_RecursiveDeref(manager, tmp);
		Cudd_RecursiveDeref(manager, symbol);

	}

	for(std::map<DdNode*, DdNode*>::iterator j = observables.begin();
			j != observables.end(); j++){
		observations->push_back((*j).second);
		printBDD((*j).second);
	}
	//cout << "done" << endl;
	return observations;
}



std::list<std::map<const pEffect*, DdNode*>*> *
observation_cpt_mtbdds(const Action& action,
		const Problem& problem){
  //std::set<std::map<const pEffect*, DdNode*, effcomp>*> combined_cpts;


  //  const Atom *atom;
  const pEffect* eff;
  ObservationEntry *e;
  ObservationVector& ob = (ObservationVector&)((Observation&)((Action&)action).observation()).obVector();
  
  // std::cout << "doing obs for " << action.name() << " " << ob.size() <<std::endl;
  
  DdNode * tmp, *tmp1, *tmp2, *fr, *fr1;
  std::list<std::map<const pEffect*, DdNode*, effcomp >*> *observations = new std::list<std::map<const pEffect*, DdNode*, effcomp>*>();

  std::map<const pEffect*, DdNode*, effcomp>* cpt;

  for(ObservationVector::iterator i =ob.begin(); i != ob.end(); i++){
    //TODO if next cpt, then make new cpt
    e = ((ObservationEntry*)(*i));
    ProbabilisticEffect obs = e->obs();

    //check existing cpts to see if one exists yet
    bool found = false;
    for(std::list<std::map<const pEffect*, DdNode*, effcomp>*>::iterator j = observations->begin();
	j != observations->end(); j++){
      //does there exist at least one of the observables in the cpt distribution
      for(int k = 0; k < obs.size(); k++){
	const pEffect* eff = &obs.effect(k);
	std::map<const pEffect*, DdNode*, effcomp>::iterator q = (*j)->find(eff);
	if(q != (*j)->end()){
	  found = true;
	  cpt = *j;
	  break;
	}
	//check if negation is in cpt
	const AddEffect *ae = dynamic_cast<const AddEffect*>(eff);
	const DeleteEffect *de = dynamic_cast<const DeleteEffect*>(eff);
	pEffect *negatedEffect;
	if(ae != NULL){
	  negatedEffect = new DeleteEffect(ae->atom());
	}
	else if(de != NULL){
	  negatedEffect = new AddEffect(de->atom());
	}
	q = (*j)->find(negatedEffect);
	if(q != (*j)->end()){
	  found = true;
	  cpt = *j;
	  break;
	}
      }

    }
    if(!found){
      //std::cout << "New cpt" << std::endl;
      cpt =  new std::map<const pEffect*, DdNode*, effcomp>();
      observations->push_back(cpt);
    }



    
    fr = formula_bdd(e->formula());
    Cudd_Ref(fr);
    tmp1 = Cudd_BddToAdd(manager, fr);
    Cudd_Ref(tmp1);
    Cudd_RecursiveDeref(manager, fr);



    //std::cout << "eff" << std::endl;
    for(int j = 0 ; j < obs.size(); j++){
      double pr = obs.probability(j).double_value();
      eff = &(obs.effect(j));
      tmp2 = Cudd_addConst(manager, pr);
      Cudd_Ref(tmp2);

 //      eff->print(std::cout, problem.domain().predicates(),
// 		 problem.domain().functions(),
// 		 problem.terms());
      //      printBDD(tmp1);
      //xprintBDD(tmp2);
      
      fr = Cudd_addApply(manager, Cudd_addTimes, tmp1, tmp2);
      Cudd_Ref(fr);
      Cudd_RecursiveDeref(manager, tmp2);

      std::map<const pEffect*, DdNode*, effcomp>::iterator k = cpt->find(eff);
      if(k != cpt->end()){
	//      fr1 = (*cpt)[eff];
	//printBDD(fr);
	//printBDD(fr1);
	
	//      if(fr1 != NULL){
	//std::cout << "found" << std:: endl;
	fr1 = (*k).second;
	tmp = Cudd_addApply(manager, Cudd_addMaximum, fr, fr1);
	Cudd_Ref(tmp);
	Cudd_RecursiveDeref(manager, fr1);
	//(*cpt)[eff] = tmp;
	//	Cudd_RecursiveDeref(manager, fr1);
	(*k).second = tmp;
	Cudd_RecursiveDeref(manager, fr);
      }
      else{
	//std::cout << "new" << std:: endl;
	//	printBDD(fr);
	//(*cpt)[eff] = fr;
	std::pair<const pEffect*, DdNode*>* n = new std::pair<const pEffect*, DdNode*>(eff, fr);
	cpt->insert(*n);
			}
      //      printBDD((*cpt)[eff]);
    }
    Cudd_RecursiveDeref(manager, tmp1);

  }

  //	recurse_compose_outcomes(&combined_cpts, observations, Cudd_ReadOne(manager), new std::set<const pEffect*>());
  return (std::list<std::map<const pEffect*, DdNode*>*>*)observations;
}



//std::map<const StateFormula*, DdNode*>
//std::map<DdNode*, DdNode*> *
//map<DdNode*, double>*
std::list<DdNode*>*
observation_mtbdds(const Action& action,
		const Problem& problem){
	//  std::map<const StateFormula*, DdNode*> ret;
	std::map<DdNode*, DdNode*>* ret = new std::map<DdNode*, DdNode*>();


	ObservationVector& ob = (ObservationVector&)((Observation&)((Action&)action).observation()).obVector();
	ObservationEntry *e;




	//  std::cout << "doing obs for " << action.name() << " " << ob.size() <<std::endl;

	/* CHECK for POMDP style observations */
	e = ((ObservationEntry*)(*(ob.begin())));
	if(&(e->symbol()) != NULL){
		return pomdp_observation_mtbdds(ob, problem);
	}


	DdNode * tmp, *tmp1, *tmp2, *fr, *fr1;
	std::map<DdNode*, std::pair<double, double>*> observables;
	std::list<DdNode*> observations;
	//map<DdNode*, std::pair<double, double>*>* outcomes = new map<DdNode*, std::pair<double, double>*>();
	std::list<DdNode*>* observFns = new std::list<DdNode*>();



	//compose all joint outcomes and compute their probabilitie

	for(ObservationVector::iterator i = //(const struct ObservationEntry**)
			ob.begin();
			i != ob.end(); i++){

		e = ((ObservationEntry*)(*i));
		fr = formula_bdd(e->formula());
		Cudd_Ref(fr);
		observables[fr] = new std::pair<double, double>(e->posProbability().double_value(),
				e->negProbability().double_value());

		//  cout << "((" << e->probability().double_value() << "))" <<endl;
	}

	// combine_observables(observables

	recurse_compose_outcomes(&observables, &observations, Cudd_ReadOne(manager));



	for(std::list<DdNode*>::iterator j = observations.begin();
			j != observations.end(); j++){



		DdNode* obsV = Cudd_ReadOne(manager);//Cudd_BddToAdd(manager, *j);
		Cudd_Ref(obsV);

		//   std::cout << "obsV:"<<endl;
		//  printBDD(obsV);

		for(std::map<DdNode*, std::pair<double, double>*>::iterator i = observables.begin();
				problem.domain().requirements.probabilistic && i != observables.end(); i++){
			std::pair<DdNode*, std::pair<double, double>*> obs = *i;


			//     std::cout << "obs:"<<std::endl;
			//          printBDD(obs.first);

			tmp1 = Cudd_ReadZero(manager);
			Cudd_Ref(tmp1);


			//probabilities that observation is right or wrong
			DdNode* posRight = Cudd_addConst(manager, (double)obs.second->first);
			Cudd_Ref(posRight);
			DdNode* posWrong = Cudd_addConst(manager, (1.0-(double)obs.second->first));
			Cudd_Ref(posWrong);
			DdNode* negRight = Cudd_addConst(manager, (double)obs.second->second);
			Cudd_Ref(negRight);
			DdNode* negWrong = Cudd_addConst(manager, (1.0-(double)obs.second->second));
			Cudd_Ref(negWrong);

			//the positive outcome in current state or in outcome
			DdNode* posB = Cudd_BddToAdd(manager, obs.first);
			Cudd_Ref(posB);
			DdNode* posA =  Cudd_addPermute(manager, posB, varmap);
			Cudd_Ref(posA);

			//the negative outcome in current state or in outcome
			DdNode* negB = Cudd_BddToAdd(manager, Cudd_Not(obs.first));
			Cudd_Ref(negB);
			DdNode* negA =  Cudd_addPermute(manager, negB, varmap);
			Cudd_Ref(negA);

			//     printBDD(obs.first);
			//     printBDD(posA);
			//     printBDD(Cudd_Not(obs.first));
			//     printBDD(negA);
			//      printBDD(right);
			//      printBDD(wrong);
			if(bdd_entailed(manager, *j, obs.first)){
				//	std::cout << "TP " << posRight << std::endl;
				//obs is observed true, and is true
				DdNode* obsTT = Cudd_addApply(manager, Cudd_addTimes, posA, posRight);
				Cudd_Ref(obsTT);
				tmp = Cudd_addApply(manager, Cudd_addPlus, tmp1, obsTT);
				Cudd_Ref(tmp);
				Cudd_RecursiveDeref(manager, tmp1);
				tmp1 = tmp;
				Cudd_Ref(tmp1);
				Cudd_RecursiveDeref(manager, tmp);
				Cudd_RecursiveDeref(manager, obsTT);
			}

			if(bdd_entailed(manager, *j, Cudd_Not(obs.first))){
				///	std::cout << "TF " << posWrong << std::endl;
				//obs is observed true, and is false
				DdNode* obsTF = Cudd_addApply(manager, Cudd_addTimes, posA, posWrong);
				Cudd_Ref(obsTF);
				tmp = Cudd_addApply(manager, Cudd_addPlus, tmp1, obsTF);
				Cudd_Ref(tmp);
				Cudd_RecursiveDeref(manager, tmp1);
				tmp1 = tmp;
				Cudd_Ref(tmp1);
				Cudd_RecursiveDeref(manager, tmp);
				Cudd_RecursiveDeref(manager, obsTF);
			}

			if(bdd_entailed(manager, *j, obs.first)){
				///	std::cout << "FT " << negWrong << std::endl;
				//obs is observed false, and is true
				DdNode* obsFT = Cudd_addApply(manager, Cudd_addTimes, negA, negWrong);
				Cudd_Ref(obsFT);
				tmp = Cudd_addApply(manager, Cudd_addPlus, tmp1, obsFT);
				Cudd_Ref(tmp);
				Cudd_RecursiveDeref(manager, tmp1);
				tmp1 = tmp;
				Cudd_Ref(tmp1);
				Cudd_RecursiveDeref(manager, tmp);
				Cudd_RecursiveDeref(manager, obsFT);
			}

			if(bdd_entailed(manager, *j, Cudd_Not(obs.first))){
				//	std::cout << "FF " << negRight << std::endl;
				//obs is observed false, and is false
				DdNode* obsFF = Cudd_addApply(manager, Cudd_addTimes, negA, negRight);
				Cudd_Ref(obsFF);
				tmp = Cudd_addApply(manager, Cudd_addPlus, tmp1, obsFF);
				Cudd_Ref(tmp);
				Cudd_RecursiveDeref(manager, tmp1);
				tmp1 = tmp;
				Cudd_Ref(tmp1);
				Cudd_RecursiveDeref(manager, tmp);
				Cudd_RecursiveDeref(manager, obsFF);
			}
			//    cout <<"["<<endl;
			//    Cudd_CheckKeys(manager);
			//    cout <<"|"<<endl;

			//       cout << "HO"<<endl;
			//       printBDD(tmp1);
			//       printBDD(obsV);

			tmp = Cudd_addApply(manager, Cudd_addTimes, tmp1, obsV);
			Cudd_Ref(tmp);
			Cudd_RecursiveDeref(manager, obsV);
			obsV = tmp;
			Cudd_Ref(obsV);
			Cudd_RecursiveDeref(manager, tmp);

			//   Cudd_CheckKeys(manager);
			//    cout <<"]"<<endl;

			Cudd_RecursiveDeref(manager, posRight);
			Cudd_RecursiveDeref(manager, posWrong);
			Cudd_RecursiveDeref(manager, negRight);
			Cudd_RecursiveDeref(manager, negWrong);
			Cudd_RecursiveDeref(manager, posA);
			Cudd_RecursiveDeref(manager, posB);
			Cudd_RecursiveDeref(manager, negA);
			Cudd_RecursiveDeref(manager, negB);



		}

		if(problem.domain().requirements.probabilistic){
			//swap vars
			tmp1 = Cudd_addPermute(manager, obsV, varmap);
			Cudd_Ref(tmp1);
			Cudd_RecursiveDeref(manager, obsV);
		}
		else{
			tmp1 = (*j);
			Cudd_Ref(tmp1);
		}
		//store observation

		//printBDD(tmp1);
		if(problem.domain().requirements.probabilistic)
			observFns->push_back(tmp1);
		else{
			observFns->push_back(tmp1);
		}
		//      std::cout << "obsVE:"<<std::endl;
		// printBDD(tmp1);



	}
	// printBDD(observFn);

	for(std::map<DdNode*, std::pair<double, double>*>::iterator i = observables.begin();
			i != observables.end(); i++){
		Cudd_RecursiveDeref(manager, (*i).first);
		delete (*i).second;
	}
	for(std::list<DdNode*>::iterator i = observations.begin();
			i != observations.end(); i++){
		Cudd_RecursiveDeref(manager, (*i));
	}

	return observFns;




	//   //for every observation formula
	//   for(ObservationVector::iterator i = (const ObservationEntry**)ob.begin();
	//       i != ob.end(); i++){

	//     //get the formula as an ADD weigted by its probability of being observed
	//     //correctly
	//     e = ((ObservationEntry*)(*i));
	//     fr = formula_bdd(e->formula());
	//     Cudd_Ref(fr);
	//     tmp = Cudd_BddToAdd(manager, fr);
	//     Cudd_Ref(tmp);
	//     // printBDD(tmp);
	//     Cudd_RecursiveDeref(manager, fr);

	//     tmp1 = Cudd_addConst(manager, e->probability().double_value());
	//     Cudd_Ref(tmp1);
	//     //printBDD(tmp1);

	//     tmp2 = Cudd_addApply(manager, Cudd_addTimes, tmp1, tmp);
	//     Cudd_Ref(tmp2);
	//     //printBDD(tmp2);
	//     //cout <<endl;

	//      Cudd_RecursiveDeref(manager, tmp);
	//     Cudd_RecursiveDeref(manager, tmp1);
	//     //     cout << "sym "<<endl;
	//      //     e->symbol().print(cout, ((Domain)problem.domain()).observables(),
	//      // 		      problem.domain().functions(),
	//      // 		      problem.terms());




	//     fr1 =  formula_bdd(e->symbol());
	//     Cudd_Ref(fr1);

	//     //     printBDD(fr1);
	//     // std::map<const StateFormula*, DdNode*>::iterator j = ret.find(&e->symbol());
	//     std::map<DdNode*, DdNode*>::iterator j = (*ret).find(fr1);

	//     //check repeats
	//     //  for(std::map<DdNode*, DdNode*>::iterator i =  ret->begin();
	//     //      i != ret->end(); i++){
	//     //    if(tmp2 == (*i).second){
	//     //      cout << "REPEAT" <<endl;
	//     //      printBDD((*i).first);
	//     //    }
	//     //  }


	//     if(j == ret->end()){
	//       //ret.insert(*new std::pair<const StateFormula*, DdNode*>(&e->symbol(),tmp2));
	//       // cout << "New"<<endl;
	//       //ret[&e->symbol()] = tmp2;
	//       (*ret)[fr1] = tmp2;

	//     }
	//     else{
	//       // cout << "Old"<<endl;
	//       tmp = Cudd_addApply(manager, Cudd_addPlus, tmp2, (*j).second);
	//       Cudd_Ref(tmp);
	//       Cudd_RecursiveDeref(manager, (*j).second);
	//       Cudd_RecursiveDeref(manager, tmp2);
	//       (*j).second = tmp;
	//       Cudd_Ref((*j).second);
	//       Cudd_RecursiveDeref(manager, tmp);
	//     }



	//   }
	// //   Cudd_CheckKeys(manager);
	// //   cout <<"]"<<endl;


	//   // for(std::map<DdNode*, DdNode*>::iterator j = ret->begin();
	//   //   j != ret->end(); j++)
	//   //    printBDD((*j).second);

	//   if(ret->size() > 0)
	//     return ret;
	//   else{
	//     delete ret;
	//     return NULL;
	//   }

}



/* Constructs MTBDDs representing the transition probability matrix and
   reward vector for the given action. */
//static
std::pair<DdNode*, DdNode*> action_mtbdds(const Action& action,
		const Problem& problem,
		DdNode* ddng,
		DdNode* ddgp,
		DdNode* col_cube,
		bool event) {
	/*
	 * Collect probabilistic outcomes for the action, and make sure that
	 * all transition sets for each outcome have mutually exclusive
	 * conditions.
	 */
	//       action.print(cout, problem.terms());     cout <<endl;
	//      action.precondition().print(cout, problem.domain().predicates(),
	// 				 problem.domain().functions(),
	// 				 problem.terms());
	//    action.effect().print(std::cout, problem.domain().predicates(),
	//    		problem.domain().functions(),
	//    		      problem.terms());
	//    std::cout << std::endl;

	//printBDD(ddng);


	OutcomeSet* outcomes = new OutcomeSet();
	DdNode *ddc;
	if(!event)
		ddc = action_preconds[&action];//formula_bdd(action.precondition());
	else
		ddc = event_preconds[&action];//formula_bdd(action.precondition());
	Cudd_Ref(ddc);

	//  cout << "getting outcomes" <<endl;
	effect_outcomes(*outcomes, ddc, action.effect());
	//  std::cout << "got outcomes" <<std::endl;

	if(!LUGTOTEXT)
		ensure_mutex(*outcomes);
	else{
		for (TransitionSetList::const_iterator ti =
				outcomes->transitions[0].begin();
				ti != outcomes->transitions[0].end(); ti++) {
			num_alt_effs++;
		}
		num_alt_effs++;//for unconditional
	}
	std::cout << "got mutex" << std::endl;

	if(!event)
		action_outcomes.insert(std::pair<const Action*, OutcomeSet*>(&action, outcomes));
	else
		event_outcomes.insert(std::pair<const Action*, OutcomeSet*>(&action, outcomes));


	if (verbosity >= 3) {
		std::cout << std::endl << "Outcomes for ";
		action.print(std::cout, problem.terms());
		std::cout << ':' << std::endl;
		print_outcomes(std::cout, *outcomes);
	}

	if(LUGTOTEXT)
		return std::make_pair(Cudd_ReadZero(manager), Cudd_ReadZero(manager));

	/*
	 * Construct MTBDD representations of the transition probability
	 * matrix and transition reward matrix for the action.
	 *
	 * The transition probability matrix for an action is the sum of the
	 * transition probability matrices of each probabilistic outcome.
	 *
	 * The transition reward matrix for an action is NOT simply the sum
	 * of the transition reward matrices for each probabilistic outcome.
	 * Each probabilistic outcome associates rewards with a set of state
	 * transitions (a subset of all state transitions).  The reward for
	 * a specific state transition must be the same for all
	 * probabilistic outcomes that associate a reward with that state
	 * transition.  An exception is thrown if the action defines
	 * inconsistent transition rewards across probabilistic outcomes.
	 * The reward for a state transition in the transition reward matrix
	 * for the action is the reward defined by some probabilistic
	 * outcome.
	 */
	/* This is going to be the transition probability matrix. */
	DdNode* ddP = Cudd_ReadZero(dd_man);
	Cudd_Ref(ddP);
	/* This is going to be the transition reward matrix. */
	DdNode* ddR = Cudd_ReadZero(dd_man);
	Cudd_Ref(ddR);
	/* This is a BDD representing all transitions by any outcome, and is
     used to accurately compute the transition reward matrix. */
	DdNode* ddD = Cudd_ReadLogicZero(dd_man);
	Cudd_Ref(ddD);
	size_t n = outcomes->probabilities.size();


	// Cudd_CheckKeys(manager);

	for (size_t i = 0; i < n; i++) {

		/*
		 * Construct MTBDD representations of the transition probability
		 * and reward matrices for the ith probabilistic outcome.
		 */
		/* This is going to be a BDD representing the state transitions
       defined by this outcome. */
		DdNode* ddT = Cudd_ReadLogicZero(dd_man);
		Cudd_Ref(ddT);


		/* This is going to be an MTBDD representing the transition reward
       matrix for this outcome. */
		DdNode* ddr;
		if (valid_reward_function) {
			ddr = Cudd_ReadZero(dd_man);
			Cudd_Ref(ddr);
		}
		/* This is going to be a BDD representing the conjunction of the
       negations of each individual transition set condition.  We need
       to add self-loops for all states satisfying this formula (this
       makes the transition set conditions not only mutually
       exclusive, but also exhaustive). */
		DdNode* ddN = Cudd_ReadOne(dd_man);
		Cudd_Ref(ddN);

		for (TransitionSetList::const_iterator ti =
				outcomes->transitions[i].begin();
				ti != outcomes->transitions[i].end(); ti++) {

			/*
			 * Process an individual transition set.
			 */
			int err = 0;
			const TransitionSet& t = **ti;
			if (t.effect_bdd() == Cudd_ReadLogicZero(dd_man)) {
				/*
				 * The effect of this transition set is inconsistent, meaning
				 * that the same atom is both added and deleted.
				 */
				//cout << "ERROR: same atom added and deleted"<<endl;
				//	   action_outcomes.erase(&action);
				//continue;
				//   free_outcomes(*outcomes);
				err = 1;
				//throw logic_error("action `" + action.name()
				//+ "' has inconsistent effects");
			}

			if(event && t.effect_bdd() == Cudd_ReadOne(dd_man))
				err = 1;


			/*
			 * The BDD representation of the transition matrix for a single
			 * transition set is computed as the conjunction of the
			 * condition BDD, the effect BDD, and the identity BDDs for each
			 * untouched state variable.
			 */
			DdNode* ddt = Cudd_ReadOne(dd_man);
			Cudd_Ref(ddt);
			//  cout << "Effect:"<<endl;
			//printBDD(t.effect_bdd());
			/*
			 * First account for untouched state variables.
			 */
			//     cout << "[" << endl;
			//      Cudd_CheckKeys(manager);
			if(!event){
				for (int vi = nvars - 1; vi >= 0; vi--) {
					if (t.touched_variables().find(vi) == t.touched_variables().end()) {
						DdNode* ddi = Cudd_bddAnd(dd_man, identity_bdds[vi], ddt);
						Cudd_Ref(ddi);
						Cudd_RecursiveDeref(dd_man, ddt);
						ddt = ddi;
						// 	  Cudd_Ref(ddt);
						// 	  Cudd_RecursiveDeref(dd_man, ddi);
					}
				}
			}
			//   cout << "|" << endl;
			//      Cudd_CheckKeys(manager);
			//      cout << "]" <<endl;

			DdNode *dda;
			if(!err){
				/*
				 * Next, the effect.
				 */
				//       cout << "anding effect"<<endl;
				dda = Cudd_bddAnd(dd_man, t.effect_bdd(), ddt);
				Cudd_Ref(dda);
				Cudd_RecursiveDeref(dd_man, ddt);
				ddt = dda;
				/*
				 * Finally, the condition.
				 */
				dda = Cudd_bddAnd(dd_man, t.condition_bdd(), ddt);
				Cudd_Ref(dda);
				Cudd_RecursiveDeref(dd_man, ddt);
				ddt = dda;


				/*
				 * Add the transition matrix for the current transition set to
				 * the transition matrix for the outcome.
				 */
				//      cout << "Oring effect"<<endl;
				DdNode* ddo = Cudd_bddOr(dd_man, ddt, ddT);
				Cudd_Ref(ddo);
				Cudd_RecursiveDeref(dd_man, ddT);
				ddT = ddo;

				//      printBDD(ddt);

				if(my_problem->domain().requirements.rewards){
					/*
					 * Add the reward for this transition set to the transition
					 * reward matrix for the outcome.
					 */
					if (valid_reward_function && t.reward() != 0.0) {
						DdNode* dds = Cudd_BddToAdd(dd_man, ddt);

						Cudd_Ref(dds);
						DdNode* ddv = Cudd_addConst(dd_man, t.reward().double_value());
						Cudd_Ref(ddv);
						dda = Cudd_addApply(dd_man, Cudd_addTimes, dds, ddv);
						Cudd_Ref(dda);
						Cudd_RecursiveDeref(dd_man, dds);
						Cudd_RecursiveDeref(dd_man, ddv);
						ddo = Cudd_addApply(dd_man, Cudd_addPlus, dda, ddr);
						Cudd_Ref(ddo);
						Cudd_RecursiveDeref(dd_man, dda);
						Cudd_RecursiveDeref(dd_man, ddr);
						ddr = ddo;
					}
				}
				Cudd_RecursiveDeref(dd_man, ddt);


			}
			else{//err - inconsistent effect
				/*
				 * Finally, the condition.
				 */
				dda = Cudd_bddAnd(dd_man, Cudd_Not(t.condition_bdd()), ddt);
				Cudd_Ref(dda);
				Cudd_RecursiveDeref(dd_man, ddt);
				ddt = dda;

			}

			/*
			 * Remove the condition for the current transition set from the
			 * condition representing uncovered states.
			 */
			DdNode* ddn = Cudd_Not(t.condition_bdd());

			Cudd_Ref(ddn);
			dda = Cudd_bddAnd(dd_man, ddn, ddN);
			Cudd_Ref(dda);
			Cudd_RecursiveDeref(dd_man, ddn);
			Cudd_RecursiveDeref(dd_man, ddN);
			ddN = dda;

		}

		/*
		 * Add self-loops for all states not covered by any transition set
		 * conditions.
		 */
		bool PPDDLTOST=false;
		DdNode* dda;
		if(!PPDDLTOST){

			dda = Cudd_bddAnd(dd_man, ddc, ddN);
			Cudd_Ref(dda);
			Cudd_RecursiveDeref(dd_man, ddN);
			if (dda != Cudd_ReadLogicZero(dd_man)) {
				ddN = dda;
				dda = Cudd_bddAnd(dd_man, ddN, identity_bdd);
				Cudd_Ref(dda);
				Cudd_RecursiveDeref(dd_man, ddN);
				ddN = dda;
				DdNode* ddo = Cudd_bddOr(dd_man, ddN, ddT);
				Cudd_Ref(ddo);
				Cudd_RecursiveDeref(dd_man, ddN);
				Cudd_RecursiveDeref(dd_man, ddT);
				ddT = ddo;
			} else {
				Cudd_RecursiveDeref(dd_man, dda);
			}
		}
		/*
		 * Multiply the transition matrix for the current outcome with the
		 * probability of the outcome, and add the result to the
		 * transition probability matrix for the action.
		 */
		//    cout << "pr = " << outcomes->probabilities[i].double_value() << endl;
		//    printBDD(ddT);
		DdNode* ddp = Cudd_BddToAdd(dd_man, ddT);
		Cudd_Ref(ddp);
		DdNode* ddk = Cudd_addConst(dd_man,
				outcomes->probabilities[i].double_value());
		Cudd_Ref(ddk);
		DdNode* ddt = Cudd_addApply(dd_man, Cudd_addTimes, ddp, ddk);
		Cudd_Ref(ddt);
		Cudd_RecursiveDeref(dd_man, ddp);
		Cudd_RecursiveDeref(dd_man, ddk);
		ddp = Cudd_addApply(dd_man, Cudd_addPlus, ddt, ddP);
		Cudd_Ref(ddp);
		Cudd_RecursiveDeref(dd_man, ddt);
		Cudd_RecursiveDeref(dd_man, ddP);
		ddP = ddp;
		//    printBDD(ddP);
		//dan     /*
		//      * Construct transition reward matrix from goal condition for goal
		//      * directed problems.
		//      */
		//     if (!valid_reward_function) {
		//       DdNode* dda = Cudd_bddAnd(dd_man, ddT, ddgp);
		//       Cudd_Ref(dda);
		//       ddr = Cudd_BddToAdd(dd_man, dda);
		//       Cudd_Ref(ddr);
		//       Cudd_RecursiveDeref(dd_man, dda);
		//     } else if (problem.goal_reward() != NULL) {
		//       Rational gr = problem.goal_reward()->expression().value(ValueMap());
		//       DdNode* ddgr = Cudd_bddAnd(dd_man, ddT, ddgp);
		//       Cudd_Ref(ddgr);
		//       dda = Cudd_BddToAdd(dd_man, ddgr);
		//       Cudd_Ref(dda);
		//       Cudd_RecursiveDeref(dd_man, ddgr);
		//       DdNode* ddk = Cudd_addConst(dd_man, gr.double_value());
		//       Cudd_Ref(ddk);
		//       ddgr = Cudd_addApply(dd_man, Cudd_addTimes, dda, ddk);
		//       Cudd_Ref(ddgr);
		//       Cudd_RecursiveDeref(dd_man, dda);
		//       Cudd_RecursiveDeref(dd_man, ddk);
		//       dda = Cudd_addApply(dd_man, Cudd_addPlus, ddgr, ddr);
		//       Cudd_Ref(dda);
		//       Cudd_RecursiveDeref(dd_man, ddgr);
		//       Cudd_RecursiveDeref(dd_man, ddr);
		//       ddr = dda;
		//     }

		if(my_problem->domain().requirements.rewards){
			/*
			 * Find the transitions that are assigned reward by the current
			 * outcome, and have also been assigned reward by previous
			 * outcomes.  The rewards are not allowed to be different for the
			 * same transition in different outcomes.
			 */
			dda = Cudd_bddAnd(dd_man, ddT, ddD);
			Cudd_Ref(dda);
			DdNode* ddn = Cudd_Not(dda);
			Cudd_Ref(ddn);
			ddt = Cudd_BddToAdd(dd_man, dda);
			Cudd_Ref(ddt);
			Cudd_RecursiveDeref(dd_man, dda);
			DdNode* new_r = Cudd_addApply(dd_man, Cudd_addTimes, ddt, ddr);
			Cudd_Ref(new_r);
			DdNode* old_r = Cudd_addApply(dd_man, Cudd_addTimes, ddt, ddR);
			Cudd_Ref(old_r);
			Cudd_RecursiveDeref(dd_man, ddt);
			if (Cudd_EqualSupNorm(dd_man, new_r, old_r, 1e-10, 0) == 0) {
				/*
				 * The current outcome assigns rewards to transitions in
				 * conflict with reward assignments of previous outcomes.
				 */
				throw std::logic_error("action `" + action.name()
						+ "' has inconsistent transition rewards");
			}
			Cudd_RecursiveDeref(dd_man, new_r);
			Cudd_RecursiveDeref(dd_man, old_r);


			/*
			 * Find the transitions that are assigned reward by the current
			 * outcome, but have not previously been assigned any reward.  Add
			 * the reward for these transitions to the transition reward
			 * matrix for the action.
			 */
			dda = Cudd_bddAnd(dd_man, ddT, ddn);
			Cudd_Ref(dda);
			Cudd_RecursiveDeref(dd_man, ddn);
			ddt = Cudd_BddToAdd(dd_man, dda);
			Cudd_Ref(ddt);
			Cudd_RecursiveDeref(dd_man, dda);
			new_r = Cudd_addApply(dd_man, Cudd_addTimes, ddt, ddr);
			Cudd_Ref(new_r);
			Cudd_RecursiveDeref(dd_man, ddt);
			Cudd_RecursiveDeref(dd_man, ddr);
			ddr = Cudd_addApply(dd_man, Cudd_addPlus, new_r, ddR);
			Cudd_Ref(ddr);
			Cudd_RecursiveDeref(dd_man, new_r);
			Cudd_RecursiveDeref(dd_man, ddR);
			ddR = ddr;
		}

		/*
		 * Add the transitions of this outcome to the BDD representing all
		 * transitions.
		 */
		DdNode* ddo = Cudd_bddOr(dd_man, ddT, ddD);
		Cudd_Ref(ddo);
		Cudd_RecursiveDeref(dd_man, ddD);
		ddD = ddo;
		Cudd_RecursiveDeref(dd_man, ddT);
	}



	Cudd_RecursiveDeref(dd_man, ddD);
	//DAN -- need them for pg construction//  free_outcomes(outcomes);
	if (verbosity >= 4) {
		std::cout << std::endl << "Transition probability matrix for ";
		action.print(std::cout, problem.terms());
		std::cout << ':' << std::endl;
		Cudd_PrintDebug(dd_man, ddP, 2*nvars, 2);
	}
	Cudd_RecursiveDeref(dd_man, ddc);

	if(my_problem->domain().requirements.rewards){
		/*
		 * Compute reward vector from the transition probability matrix
		 * and transition reward matrix.
		 */
		DdNode* tmp = Cudd_addApply(dd_man, Cudd_addTimes, ddP, ddR);
		Cudd_Ref(tmp);
		Cudd_RecursiveDeref(dd_man, ddR);
		ddR = Cudd_addExistAbstract(dd_man, tmp, col_cube);
		Cudd_Ref(ddR);
		Cudd_RecursiveDeref(dd_man, tmp);
		if (verbosity >= 3) {
			std::cout << std::endl << "Reward vector\n ";
			Cudd_PrintDebug(dd_man, ddR, 2*nvars, 2);
		}
	}

	if(problem.domain().requirements.non_deterministic){
		DdNode *nd = Cudd_addBddStrictThreshold(manager,ddP,0.0);
		Cudd_Ref(nd);
		Cudd_RecursiveDeref(manager, ddP);
		ddP = nd;
		Cudd_Ref(ddP);
		Cudd_RecursiveDeref(manager, nd);
	}

	return std::make_pair(ddP, ddR);
}


/* ====================================================================== */
/* Value iteration. */

/*
 * Returns a policy for the current problem generated using value
 * iteration.
 */
static DdNode* value_iteration(const Problem& problem,
		DdNode* ddng, DdNode* col_cube,
		double gamma, double epsilon) {
	if (verbosity >= 3) {
		std::cout << "Value iteration";
	}
	/*
	 * Precompute variable permutations.
	 */
	int* row_to_col = new int[2*nvars];
	int* col_to_row = new int[2*nvars];
	for (int i = 0; i < nvars; i++) {
		row_to_col[2*i] = 2*i + 1;
		row_to_col[2*i + 1] = 2*i + 1;
		col_to_row[2*i] = 2*i;
		col_to_row[2*i + 1] = 2*i;
	}

	/*
	 * Construct action conditions, action value filters, and initial policy.
	 */
	std::map<const Action*, DdNode*> filters;
	std::map<const Action*, DdNode*> policy;
	for (std::map<const Action*, DdNode*>::const_iterator ai =
			action_transitions.begin();
			ai != action_transitions.end(); ai++) {
		DdNode* ddc = formula_bdd((*ai).first->precondition());
		DdNode* ddt = Cudd_bddAnd(dd_man, ddc, ddng);
		Cudd_Ref(ddt);
		Cudd_RecursiveDeref(dd_man, ddc);
		DdNode* ddn = Cudd_Not(ddt);
		Cudd_Ref(ddn);
		Cudd_RecursiveDeref(dd_man, ddt);
		ddc = Cudd_BddToAdd(dd_man, ddn);
		Cudd_Ref(ddc);
		Cudd_RecursiveDeref(dd_man, ddn);
		ddn = Cudd_ReadPlusInfinity(dd_man);
		Cudd_Ref(ddn);
		ddt = Cudd_addApply(dd_man, Cudd_addTimes, ddc, ddn);
		Cudd_Ref(ddt);
		Cudd_RecursiveDeref(dd_man, ddc);
		Cudd_RecursiveDeref(dd_man, ddn);
		filters.insert(std::make_pair((*ai).first, ddt));
		DdNode* ddp = Cudd_ReadLogicZero(dd_man);
		Cudd_Ref(ddp);
		policy.insert(std::make_pair((*ai).first, ddp));
	}

	/*
	 * Iterate until value function converges.
	 */
	DdNode* ddg = Cudd_addConst(dd_man, gamma);
	Cudd_Ref(ddg);
	DdNode* ddV = Cudd_ReadZero(dd_man);
	Cudd_Ref(ddV);
	if (verbosity >= 3) {
		std::cout << std::endl << "V 0:" << std::endl;
		Cudd_PrintDebug(dd_man, ddV, 2*nvars, 2);
	}
	double tolerance = epsilon*(1.0 - gamma)/(2.0*gamma);
	bool done = false;
	size_t iters = 0;
	while(!done){
		iters++;
		if(verbosity >= 3){
			if(iters % 1000 == 0)
				std::cout << ':';
			else if (iters % 100 == 0)
				std::cout << '.';
		}
		DdNode* ddVp = Cudd_addPermute(dd_man, ddV, row_to_col);
		Cudd_Ref(ddVp);
		DdNode* ddM = Cudd_ReadZero(dd_man);
		Cudd_Ref(ddM);
		for (std::map<const Action*, DdNode*>::const_iterator ai =
				action_transitions.begin();
				ai != action_transitions.end(); ai++) {
			DdNode* ddp = Cudd_addApply(dd_man, Cudd_addTimes, (*ai).second, ddVp);
			Cudd_Ref(ddp);
			DdNode* dds = Cudd_addExistAbstract(dd_man, ddp, col_cube);
			Cudd_Ref(dds);
			Cudd_RecursiveDeref(dd_man, ddp);
			ddp = Cudd_addApply(dd_man, Cudd_addTimes, ddg, dds);
			Cudd_Ref(ddp);
			Cudd_RecursiveDeref(dd_man, dds);
			dds = Cudd_addApply(dd_man, Cudd_addPlus,
					action_rewards[(*ai).first], ddp);
			Cudd_Ref(dds);
			Cudd_RecursiveDeref(dd_man, ddp);
			ddp = Cudd_addPermute(dd_man, dds, col_to_row);
			Cudd_Ref(ddp);
			Cudd_RecursiveDeref(dd_man, dds);
			DdNode* ddf = Cudd_addApply(dd_man, Cudd_addMinus, ddp,
					filters[(*ai).first]);
			Cudd_Ref(ddf);
			Cudd_RecursiveDeref(dd_man, ddp);
			ddp = ddf;
			if (verbosity >= 3) {
				std::cout << std::endl << "value of action ";
				(*ai).first->print(std::cout, problem.terms());
				std::cout << ':' << std::endl;
				Cudd_PrintDebug(dd_man, ddp, 2*nvars, 1);
			}
			DdNode* ddm = Cudd_addApply(dd_man, Cudd_addMinus, ddp, ddM);
			Cudd_Ref(ddm);
			DdNode*& dde = policy[(*ai).first];
			Cudd_RecursiveDeref(dd_man, dde);
			dde = Cudd_addBddThreshold(dd_man, ddm, 0);
			Cudd_Ref(dde);
			Cudd_RecursiveDeref(dd_man, ddm);
			DdNode* ddn = Cudd_Not(dde);
			Cudd_Ref(ddn);
			if (ddn != Cudd_ReadOne(dd_man)) {
				for (std::map<const Action*, DdNode*>::const_iterator aj =
						policy.begin();
						(*aj).first != (*ai).first; aj++) {
					DdNode*& ddj = policy[(*aj).first];
					DdNode* dda = Cudd_bddAnd(dd_man, ddn, ddj);
					Cudd_Ref(dda);
					Cudd_RecursiveDeref(dd_man, ddj);
					ddj = dda;
				}
			}
			Cudd_RecursiveDeref(dd_man, ddn);
			ddm = Cudd_addApply(dd_man, Cudd_addMaximum, ddp, ddM);
			Cudd_Ref(ddm);
			Cudd_RecursiveDeref(dd_man, ddM);
			ddM = ddm;
			if (verbosity >= 3) {
				std::cout << "current max values:" << std::endl;
				Cudd_PrintDebug(dd_man, ddM, 2*nvars, 1);
			}
			Cudd_RecursiveDeref(dd_man, ddp);
		}
		Cudd_RecursiveDeref(dd_man, ddVp);
		ddVp = ddM;
		if (verbosity >= 3) {
			std::cout << "V " << iters << ':' << std::endl;
			Cudd_PrintDebug(dd_man, ddVp, nvars, 1);
		}
		done = (Cudd_EqualSupNorm(dd_man, ddV, ddVp, tolerance, 0) == 1);
		Cudd_RecursiveDeref(dd_man, ddV);
		ddV = ddVp;
	}
	if (verbosity >= 3) {
		std::cout << ' ' << iters << " iterations." << std::endl;
	}
	Cudd_RecursiveDeref(dd_man, ddV);
	Cudd_RecursiveDeref(dd_man, ddg);
	for (std::map<const Action*, DdNode*>::const_iterator ai = filters.begin();
			ai != filters.end(); ai++) {
		Cudd_RecursiveDeref(dd_man, (*ai).second);
	}
	delete row_to_col;
	delete col_to_row;

	/*
	 * Construct single policy MTBDD.
	 */
	DdNode* ddP = Cudd_ReadZero(dd_man);
	Cudd_Ref(ddP);
	for (std::map<const Action*, DdNode*>::const_iterator ai = policy.begin();
			ai != policy.end(); ai++) {
		if ((*ai).second != Cudd_ReadLogicZero(dd_man)) {
			policy_actions.insert(std::make_pair((*ai).first->id(), (*ai).first));
		}
		DdNode* ddp = Cudd_BddToAdd(dd_man, (*ai).second);
		Cudd_Ref(ddp);
		Cudd_RecursiveDeref(dd_man, (*ai).second);
		size_t id = (*ai).first->id();
		DdNode* ddi = Cudd_addConst(dd_man, id);
		Cudd_Ref(ddi);
		DdNode* ddt = Cudd_addApply(dd_man, Cudd_addTimes, ddi, ddp);
		Cudd_Ref(ddt);
		Cudd_RecursiveDeref(dd_man, ddi);
		Cudd_RecursiveDeref(dd_man, ddp);
		ddp = Cudd_addApply(dd_man, Cudd_addPlus, ddt, ddP);
		Cudd_Ref(ddp);
		Cudd_RecursiveDeref(dd_man, ddt);
		Cudd_RecursiveDeref(dd_man, ddP);
		ddP = ddp;
	}
	return ddP;
}


DdNode* iterative_assemble_init(std::list<std::list<DdNode*>* >* preffects,
		std::list<std::list<DdNode*>* >::iterator p,
		DdNode* my_initial_state,
		DdNode* model){
	DdNode* result = my_initial_state;
	Cudd_Ref(result);
	for(;p != preffects->end(); p++){
		//    DdNode *t = Cudd_ReadZero(manager);
		//    Cudd_Ref(t);
		std::list<DdNode*> r;
		for(std::list<DdNode*>::iterator o = (*p)->begin();
				o != (*p)->end(); o++){
			DdNode *t1 = Cudd_addApply(manager,
					Cudd_addTimes,
					result,
					*o);
			Cudd_Ref(t1);
			r.push_back(t1);
			//       Cudd_RecursiveDeref(manager, t);
			//       t = t1;
			//       Cudd_Ref(t);
			//       Cudd_RecursiveDeref(manager, t1);
			//              std::cout << "o, t"<< std::endl;
			//        printBDD(*o);
			//        printBDD(t1);
		}
		Cudd_RecursiveDeref(manager, result);
		result = Cudd_ReadZero(manager);
		Cudd_Ref(result);
		for(std::list<DdNode*>::iterator o = r.begin();
				o != r.end(); o++){
			DdNode *t1 = Cudd_addApply(manager,
					Cudd_addPlus,
					result,
					*o);
			Cudd_Ref(t1);
			Cudd_RecursiveDeref(manager, result);
			result = t1;
			Cudd_Ref(result);
			Cudd_RecursiveDeref(manager, t1);
			//             std::cout << "o, t"<< std::endl;
			//       printBDD(*o);
			//       printBDD(t);
		}
		//     DdNode *t1 = Cudd_addApply(manager,
		// 			       Cudd_addTimes,
		// 			       t,
		// 			       result);
		//       Cudd_Ref(t1);
		//       Cudd_RecursiveDeref(manager, result);
		//       result = t1;
		//       Cudd_Ref(result);
		//       Cudd_RecursiveDeref(manager, t1);

		//          std::cout << "result" << std::endl;
		//       printBDD(result);
	}
	return result;

	//   DdNode* result = my_initial_state;
	//   Cudd_Ref(result);
	//   for(;p != preffects->end(); p++){
	//     DdNode *t = Cudd_ReadZero(manager);
	//     Cudd_Ref(t);
	//     for(std::list<DdNode*>::iterator o = (*p)->begin();
	// 	o != (*p)->end(); o++){
	//       DdNode *t1 = Cudd_addApply(manager,
	// 				Cudd_addPlus,
	// 				 t,
	// 				 *o);
	//       Cudd_Ref(t1);
	//       Cudd_RecursiveDeref(manager, t);
	//       t = t1;
	//       Cudd_Ref(t);
	//       Cudd_RecursiveDeref(manager, t1);
	//             std::cout << "o, t"<< std::endl;
	//       printBDD(*o);
	//       printBDD(t);
	//     }
	//     DdNode *t1 = Cudd_addApply(manager,
	// 			       Cudd_addTimes,
	// 			       t,
	// 			       result);
	//       Cudd_Ref(t1);
	//       Cudd_RecursiveDeref(manager, result);
	//       result = t1;
	//       Cudd_Ref(result);
	//       Cudd_RecursiveDeref(manager, t1);
	//          std::cout << "result" << std::endl;
	//       printBDD(result);
	//   }
	//   return result;
}

DdNode* recurse_assemble_init(std::list<std::list<DdNode*>* >* preffects,
		std::list<std::list<DdNode*>* >::iterator p,
		DdNode* my_initial_state,
		DdNode* model){
	if(p == preffects->end()){//found joint effect model
		//add joint effect to init
		//    cout << "model = " <<endl;
		//    printBDD(model);
		DdNode* tmp;
		if(my_initial_state == Cudd_ReadOne(manager)){
			tmp = model;
		}
		else{
			tmp = Cudd_addApply(manager,
					Cudd_addPlus,
					model,
					my_initial_state);
		}
		Cudd_Ref(tmp);
		//printBDD(tmp);
		return tmp;

	}
	else{//recurse over possibilities of effect
		std::list<std::list<DdNode*>* >::iterator nextP = (p);
		nextP++;
		for(std::list<DdNode*>::iterator o = (*p)->begin();
				o != (*p)->end(); o++){
			DdNode* tmp = Cudd_addApply(manager,
					Cudd_addTimes,
					model,
					*o);
			Cudd_Ref(tmp);
			DdNode* tmp1 = recurse_assemble_init(preffects, nextP, my_initial_state, tmp);
			Cudd_Ref(tmp1);
			Cudd_RecursiveDeref(manager, tmp);
			Cudd_RecursiveDeref(manager, my_initial_state);
			my_initial_state = tmp1;
			Cudd_Ref(my_initial_state);
			Cudd_RecursiveDeref(manager, tmp1);

		}
	}
	//  printBDD(my_initial_state);

	return my_initial_state;

}

void make_mutex1(std::list<DdNode*>* preffect, DdNode *varsUsed){
	//for each outcome, if not mention var, then negate
	for(std::list<DdNode*>::iterator i = preffect->begin();
			i != preffect->end(); i++){
		DdNode *tmp = Cudd_addBddThreshold(manager, *i, Cudd_ReadEpsilon(manager));
		Cudd_Ref(tmp);
		//      printBDD(*i);
		//      printBDD(tmp);
		//      printBDD(varsUsed);
		for(int j = 0; j < num_alt_facts; j++){
			if(Cudd_bddIsVarEssential(manager,varsUsed, (2*j)+1, 1) &&
					!bdd_entailed(manager, tmp, Cudd_bddIthVar(manager, (2*j)+1))){
				//!Cudd_bddIsVarEssential(manager,tmp, (2*j)+1, 1) ){
				DdNode *t = Cudd_bddAnd(manager, tmp, Cudd_Not(Cudd_bddIthVar(manager, (2*j)+1)));
				Cudd_Ref(t);
				Cudd_RecursiveDeref(manager, tmp);
				tmp = t;
				Cudd_Ref(tmp);
				Cudd_RecursiveDeref(manager, t);
			}
			//	printBDD(tmp);
		}
		DdNode* t = Cudd_BddToAdd(manager, tmp);
		Cudd_Ref(t);
		Cudd_RecursiveDeref(manager, tmp);
		tmp = t;
		Cudd_Ref(tmp);
		Cudd_RecursiveDeref(manager, t);

		t = Cudd_addApply(manager, Cudd_addTimes, tmp, *i);
		Cudd_Ref(t);
		Cudd_RecursiveDeref(manager, *i);
		*i = t;
		Cudd_Ref(*i);
		Cudd_RecursiveDeref(manager, t);
		Cudd_RecursiveDeref(manager, tmp);
		//      printBDD(*i);
	}
}

void make_mutex(std::list<DdNode*>* preffect){
	std::list<DdNode*> out;

	for(std::list<DdNode*>::iterator i = preffect->begin();
			i != preffect->end(); i++){
		std::cout << "i = " <<std::endl;
		printBDD(*i);
		DdNode* tmpd = *i;
		Cudd_Ref(tmpd);
		for(std::list<DdNode*>::iterator j = preffect->begin();
				j != preffect->end(); j++){
			if(i != j){
				std::cout << "j = " <<std::endl;
				printBDD(*j);
				DdNode* tmpi = Cudd_addBddStrictThreshold(manager, tmpd, 0.0);
				Cudd_Ref(tmpi);


				DdNode* tmpj = Cudd_addBddStrictThreshold(manager, *j, 0.0);
				Cudd_Ref(tmpj);
				DdNode *tmpnj = Cudd_Not(tmpj);
				Cudd_Ref(tmpnj);
				Cudd_RecursiveDeref(dd_man, tmpj);

				if(tmpnj == Cudd_ReadLogicZero(manager)){
					Cudd_RecursiveDeref(dd_man, tmpnj);
					tmpnj = Cudd_ReadOne(manager);
					Cudd_Ref(tmpnj);
				}


				DdNode* tmp = Cudd_bddAnd(manager, tmpi, tmpnj);
				Cudd_Ref(tmp);
				Cudd_RecursiveDeref(dd_man, tmpi);
				Cudd_RecursiveDeref(dd_man, tmpnj);
				tmpi = Cudd_BddToAdd(manager, tmp);
				Cudd_Ref(tmpi);
				Cudd_RecursiveDeref(dd_man, tmp);
				tmpj = Cudd_addApply(manager, Cudd_addTimes, tmpd, tmpi);
				Cudd_Ref(tmpj);
				Cudd_RecursiveDeref(dd_man, tmpi);
				Cudd_RecursiveDeref(dd_man, tmpd);
				tmpd = tmpj;
				Cudd_Ref(tmpd);
				Cudd_RecursiveDeref(dd_man, tmpj);
			}
		}
		out.push_back(tmpd);
		std::cout << "tmpd = " <<std::endl;
		printBDD(tmpd);
	}

	for(std::list<DdNode*>::iterator i = preffect->begin();
			i != preffect->end(); i++){
		Cudd_RecursiveDeref(manager, *i);
	}
	preffect->clear();
	preffect->merge(out);
}

/**
 *  momo007 013 2022.04.27
 * 这个函数实现了初始状态公式到obdd的转换
 * 1. collect_init_state_variable，查找所有状态变量个数
 * 2. formula_bdd，将初始状态formula转化为obdd，其中one-of disjunction没太理解
 * 3. 随后将每个状态变量的否定和区上第二部的obdd，为什么需要这一步操作？
 */
void collectInit(const Problem* problem){
	/*
	 * Construct an ADD representing initial states.
	 */

	//   problem->init_formula().print(cout, problem->domain().predicates(),
	//  				 problem->domain().functions(),
	//  				 problem->terms());
	//   cout <<endl;

	if(&problem->init_formula()){
		collect_init_state_variables(problem->init_formula());// 公式中涉及到的状态变量即atom
		DdNode* tmp = formula_bdd(problem->init_formula());// 根据初始状态公式创建BDD
		Cudd_Ref(tmp);//增加引用
		
		for(int i = 0; i < num_alt_facts; i++){//考虑每个状态变量
			const Atom *a = (*(dynamic_atoms.find(i))).second;
			if(init_variables.find(a) == init_variables.end()){
				// 创建第i个状态变量的否定，将其与初始状态和取
				DdNode* tmp1 = Cudd_bddAnd(manager, tmp, Cudd_Not(Cudd_bddIthVar(manager, 2*i)));
				Cudd_Ref(tmp1);
				Cudd_RecursiveDeref(manager, tmp);
				tmp = tmp1;
				Cudd_Ref(tmp);
				Cudd_RecursiveDeref(manager, tmp1);
			}
		}
		// 记录初始状态集合，这里为何需要合取所有状态变量的否定？
		b_initial_state = tmp;
		Cudd_Ref(b_initial_state);
		Cudd_RecursiveDeref(manager, tmp);
		return;
	}



	const Atom *a;
	if(0)  {
		//     ConjunctiveEffect* ce = new ConjunctiveEffect();
		//     for(EffectList::iterator ie=(const pEffect**)problem->init_effects().begin();
		// 	ie!= problem->init_effects().end(); ie++){
		//       ce->add_conjunct(**ie);
		//     }
		//     for(int i = 0; i < num_alt_facts; i+=1){
		//       a = (*(dynamic_atoms.find(i))).second;
		//       if(((problem)->init_atoms().find(a)) !=
		// 	 (problem)->init_atoms().end()){
		// 	ce->add_conjunct(*(new AddEffect(*a)));
		//       }
		//     }

		//     OutcomeSet* outcomes = new OutcomeSet();
		//     DdNode *ddc = Cudd_ReadOne(manager);
		//     Cudd_Ref(ddc);
		//     effect_outcomes(*outcomes, ddc, *ce);
		//     cout << "hi"<<endl;
		//     //ensure_mutex(*outcomes);

		//     /* This is going to be the transition probability matrix. */
		//     DdNode* ddP = Cudd_ReadZero(dd_man);
		//     Cudd_Ref(ddP);
		//     /* This is a BDD representing all transitions by any outcome, and is
		//        used to accurately compute the transition reward matrix. */
		//     DdNode* ddD = Cudd_ReadLogicZero(dd_man);
		//     Cudd_Ref(ddD);
		//     size_t n = outcomes->probabilities.size();
		//     for (size_t i = 0; i < n; i++) {
		//       /* This is going to be a BDD representing the state transitions
		// 	 defined by this outcome. */
		//       cout << "*";
		//       DdNode* ddT = Cudd_ReadLogicZero(dd_man);
		//       Cudd_Ref(ddT);
		//       /* This is going to be a BDD representing the conjunction of the
		// 	 negations of each individual transition set condition.  We need
		// 	 to add self-loops for all states satisfying this formula (this
		// 	 makes the transition set conditions not only mutually
		// 	 exclusive, but also exhaustive). */
		//       DdNode* ddN = Cudd_ReadOne(dd_man);
		//       Cudd_Ref(ddN);
		//       for (TransitionSetList::const_iterator ti =
		// 	     outcomes->transitions[i].begin();
		// 	   ti != outcomes->transitions[i].end(); ti++) {
		// 	cout << "-";
		// 	/*
		// 	 * Process an individual transition set.
		// 	 */
		// 	const TransitionSet& t = **ti;
		// 	if (t.effect_bdd() == Cudd_ReadLogicZero(dd_man)) {
		// 	  /*
		// 	   * The effect of this transition set is inconsistent, meaning
		// 	   * that the same atom is both added and deleted.
		// 	   */
		// 	  //cout << "ERROR: same atom added and deleted"<<endl;
		// 	  //	   action_outcomes.erase(&action);
		// 	  //continue;

		// 	  throw std::logic_error("initial belief has inconsistent effects");
		// 	}
		// 	/*
		// 	 * The BDD representation of the transition matrix for a single
		// 	 * transition set is computed as the conjunction of the
		// 	 * condition BDD, the effect BDD, and the identity BDDs for each
		// 	 * untouched state variable.
		// 	 */
		// 	DdNode* ddt = Cudd_ReadOne(dd_man);
		// 	Cudd_Ref(ddt);
		// 	/*
		// 	 * First account for untouched state variables.
		// 	 */
		// 	for (int vi = nvars - 1; vi >= 0; vi--) {
		// 	  if (t.touched_variables().find(vi) == t.touched_variables().end()) {
		// 	    DdNode* ddi = Cudd_bddAnd(dd_man, identity_bdds[vi], ddt);
		// 	    Cudd_Ref(ddi);
		// 	    Cudd_RecursiveDeref(dd_man, ddt);
		// 	    ddt = ddi;
		// 	    Cudd_Ref(ddt);
		// 	    Cudd_RecursiveDeref(dd_man, ddi);
		// 	  }
		// 	}
		//  	/*
		// 	 * Next, the effect.
		// 	 */
		// 	//       cout << "anding effect"<<endl;
		// 	DdNode* dda = Cudd_bddAnd(dd_man, t.effect_bdd(), ddt);
		// 	Cudd_Ref(dda);
		// 	Cudd_RecursiveDeref(dd_man, ddt);
		// 	ddt = dda;
		// 	/*
		// 	 * Add the transition matrix for the current transition set to
		// 	 * the transition matrix for the outcome.
		// 	 */
		// 	//      cout << "Oring effect"<<endl;
		// 	DdNode* ddo = Cudd_bddOr(dd_man, ddt, ddT);
		// 	Cudd_Ref(ddo);
		// 	Cudd_RecursiveDeref(dd_man, ddT);
		// 	ddT = ddo;
		// 	Cudd_RecursiveDeref(dd_man, ddt);
		// 	/*
		// 	 * Remove the condition for the current transition set from the
		// 	 * condition representing uncovered states.
		// 	 */
		// 	DdNode* ddn = Cudd_Not(t.condition_bdd());

		// 	Cudd_Ref(ddn);
		// 	dda = Cudd_bddAnd(dd_man, ddn, ddN);
		// 	Cudd_Ref(dda);
		// 	Cudd_RecursiveDeref(dd_man, ddn);
		// 	Cudd_RecursiveDeref(dd_man, ddN);
		// 	ddN = dda;
		//       }




		//       /*
		//        * Add self-loops for all states not covered by any transition set
		//        * conditions.
		//        */
		//       DdNode* dda;
		//       dda = Cudd_bddAnd(dd_man, ddc, ddN);
		//       Cudd_Ref(dda);
		//       Cudd_RecursiveDeref(dd_man, ddN);
		//       if (dda != Cudd_ReadLogicZero(dd_man)) {
		// 	ddN = dda;
		// 	dda = Cudd_bddAnd(dd_man, ddN, identity_bdd);
		// 	Cudd_Ref(dda);
		// 	Cudd_RecursiveDeref(dd_man, ddN);
		// 	ddN = dda;
		// 	DdNode* ddo = Cudd_bddOr(dd_man, ddN, ddT);
		// 	Cudd_Ref(ddo);
		// 	Cudd_RecursiveDeref(dd_man, ddN);
		// 	Cudd_RecursiveDeref(dd_man, ddT);
		// 	ddT = ddo;
		//       } else {
		// 	Cudd_RecursiveDeref(dd_man, dda);
		//       }

		//       /*
		//        * Multiply the transition matrix for the current outcome with the
		//        * probability of the outcome, and add the result to the
		//        * transition probability matrix for the action.
		//        */
		//       DdNode* ddp = Cudd_BddToAdd(dd_man, ddT);
		//       Cudd_Ref(ddp);
		//       DdNode* ddk = Cudd_addConst(dd_man,
		// 				  outcomes->probabilities[i].double_value());
		//       Cudd_Ref(ddk);
		//       DdNode* ddt = Cudd_addApply(dd_man, Cudd_addTimes, ddp, ddk);
		//       Cudd_Ref(ddt);
		//       Cudd_RecursiveDeref(dd_man, ddp);
		//       Cudd_RecursiveDeref(dd_man, ddk);
		//       ddp = Cudd_addApply(dd_man, Cudd_addPlus, ddt, ddP);
		//       Cudd_Ref(ddp);
		//       Cudd_RecursiveDeref(dd_man, ddt);
		//       Cudd_RecursiveDeref(dd_man, ddP);
		//       ddP = ddp;
		//       /*
		//        * Add the transitions of this outcome to the BDD representing all
		//        * transitions.
		//        */
		//       DdNode* ddo = Cudd_bddOr(dd_man, ddT, ddD);
		//       Cudd_Ref(ddo);
		//       Cudd_RecursiveDeref(dd_man, ddD);
		//       ddD = ddo;
		//       Cudd_RecursiveDeref(dd_man, ddT);

		//     }


		//     Cudd_RecursiveDeref(dd_man, ddD);
		//     Cudd_RecursiveDeref(dd_man, ddc);
		//     delete ce;
		//     free_outcomes(*outcomes);



		//     DdNode* init;
		//     if(problem->domain().requirements.non_deterministic){
		//       DdNode *nd = Cudd_addBddStrictThreshold(manager,ddP,0.0);
		//       Cudd_Ref(nd);
		//       Cudd_RecursiveDeref(manager, ddP);
		//       ddP = nd;
		//       Cudd_Ref(ddP);
		//       Cudd_RecursiveDeref(manager, nd);



		//       init = Cudd_ReadOne(manager);
		//             cout << "num facts = " << num_alt_facts<<endl;
		//       for(int k = 0; k < num_alt_facts; k++){
		// 	nd = Cudd_Not(Cudd_bddIthVar(manager, 2*k));
		// 	Cudd_Ref(nd);
		// 	ddD = Cudd_bddAnd(manager, init, nd);
		// 	Cudd_Ref(ddD);
		// 	Cudd_RecursiveDeref(manager, nd);
		// 	Cudd_RecursiveDeref(manager, init);
		// 	init = ddD;
		// 	Cudd_Ref(init);
		// 	Cudd_RecursiveDeref(manager, ddD);
		//       }
		//       // cout << "HI"<<endl;
		//       //printBDD(init);
		//       //printBDD(ddP);
		//       // printBDD(current_state_cube);

		//       nd = Cudd_bddAndAbstract(manager,init,
		// 			       ddP,current_state_cube);
		//       Cudd_Ref(nd);
		//       //      printBDD(nd);
		//       b_initial_state = Cudd_bddVarMap(manager, nd);
		//       Cudd_Ref(b_initial_state);
		//       //printBDD(b_initial_state);
		//       Cudd_RecursiveDeref(manager, nd);
		//       Cudd_RecursiveDeref(manager, init);
		//       //      return b_initial_state;


		//     }
		//     else{
		//       DdNode *nd;
		//       init = Cudd_ReadOne(manager);
		//       //      cout << "num facts = " << num_alt_facts<<endl;
		//       for(int k = 0; k < num_alt_facts; k++){
		// 	nd = Cudd_Not(Cudd_bddIthVar(manager, 2*k));
		// 	Cudd_Ref(nd);
		// 	ddD = Cudd_bddAnd(manager, init, nd);
		// 	Cudd_Ref(ddD);
		// 	Cudd_RecursiveDeref(manager, nd);
		// 	Cudd_RecursiveDeref(manager, init);
		// 	init = ddD;
		// 	Cudd_Ref(init);
		// 	Cudd_RecursiveDeref(manager, ddD);
		//       }
		//       ddD = Cudd_BddToAdd(manager, init);
		//       Cudd_Ref(ddD);
		//       Cudd_RecursiveDeref(manager, init);
		//       init = ddD;
		//       Cudd_Ref(init);
		//       Cudd_RecursiveDeref(manager, ddD);



		//       //printBDD(init);
		//       //printBDD(ddP);
		//       // printBDD(current_state_cube);

		//       nd = Cudd_addApply(manager, Cudd_addTimes, init, ddP);
		//       Cudd_Ref(nd);
		//       // cout << "HI"<<endl;
		//       ddD = Cudd_addExistAbstract(manager, nd, current_state_cube);
		//       Cudd_Ref(ddD);



		//       b_initial_state = Cudd_addPermute(manager, ddD, varmap);
		//       Cudd_Ref(b_initial_state);
		//       //printBDD(b_initial_state);
		//       Cudd_RecursiveDeref(manager, nd);
		//       Cudd_RecursiveDeref(manager, init);
		//       Cudd_RecursiveDeref(manager, ddD);
		//       //      return b_initial_state;

		//     }

	}
	else if(0){
		const ProbabilisticEffect* pe;

		//std::cout << "HI" <<std::endl;

		b_initial_state = Cudd_addConst(manager, 1.0);//Cudd_ReadOne(manager);
		Cudd_Ref(b_initial_state);
		DdNode* tmp, *tmp1, *tmp2, *fr, *pr_atoms;
		pr_atoms = Cudd_ReadOne(manager);
		EffectList& efl = (EffectList&)problem->init_effects();
		std::list<std::list<DdNode*>* > preffects;
		for(EffectList::iterator ie=//(const pEffect**)
				efl.begin();
				ie!= efl.end(); ie++){

			if(pe = dynamic_cast<const ProbabilisticEffect*>(*ie)){
				OutcomeSet outcomes;
				effect_outcomes(outcomes, Cudd_addConst(manager, 1.0), *pe);
				// print_outcomes(std::cout, outcomes);
				std::list<DdNode*>* preffect = new std::list<DdNode*>();
				preffects.push_back(preffect);
				size_t n = outcomes.probabilities.size();
				double sum = 0.0;
				for (size_t i = 0; i < n; i++) {
					// 	 sum += outcomes.probabilities[i].double_value();
					// 	 tmp = Cudd_addConst(manager, outcomes.probabilities[i].double_value());
					// 	 Cudd_Ref(tmp);
					// 	 for (TransitionSetList::const_iterator ti =
					// 		outcomes.transitions[i].begin();
					// 	      ti != outcomes.transitions[i].end(); ti++) {
					// 	   tmp1 = Cudd_BddToAdd(manager, (*ti)->effect_bdd());
					// 	   Cudd_Ref(tmp1);
					// 	   tmp2 = Cudd_addApply(manager, Cudd_addTimes, tmp1, tmp);
					// 	   Cudd_Ref(tmp2);
					// 	   Cudd_RecursiveDeref(manager, tmp1);
					// 	   preffect->push_back(tmp2);
					// 	   printBDD(tmp2);
					// 	 }

					if(outcomes.transitions[i].size() == 1 &&
							outcomes.transitions[i].front()->effect_bdd() == Cudd_ReadOne(manager))
						continue;

					sum += outcomes.probabilities[i].double_value();
					tmp = Cudd_addConst(manager, outcomes.probabilities[i].double_value());
					Cudd_Ref(tmp);
					tmp1 = Cudd_ReadOne(manager);
					for (TransitionSetList::const_iterator ti =
							outcomes.transitions[i].begin();
							ti != outcomes.transitions[i].end(); ti++) {

						tmp2 = Cudd_bddAnd(manager, tmp1, (*ti)->effect_bdd());
						Cudd_Ref(tmp2);
						Cudd_RecursiveDeref(manager, tmp1);
						tmp1 = tmp2;
						Cudd_Ref(tmp1);
						Cudd_RecursiveDeref(manager, tmp2);
					}
					fr = Cudd_BddToAdd(manager, tmp1);
					Cudd_Ref(fr);
					tmp2 = Cudd_addApply(manager, Cudd_addTimes, fr, tmp);
					Cudd_Ref(tmp2);
					Cudd_RecursiveDeref(manager, fr);
					Cudd_RecursiveDeref(manager, tmp1);
					preffect->push_back(tmp2);
					//printBDD(tmp2);

					tmp1 = Cudd_Support(manager, tmp2);
					Cudd_Ref(tmp1);
					fr = Cudd_bddAnd(manager, pr_atoms, tmp1);
					Cudd_Ref(fr);
					Cudd_RecursiveDeref(manager, pr_atoms);
					pr_atoms = fr;
					Cudd_Ref(pr_atoms);
					Cudd_RecursiveDeref(manager, fr);
				}


				if(sum < 1.0){//need to add case where all given effects are false
					//std::cout << "Sum lt 1"<<std::endl;
					tmp2 = Cudd_ReadOne(manager);
					Cudd_Ref(tmp2);

					// 	 for (size_t i = 0; i < n; i++) {
					// 	   for (TransitionSetList::const_iterator ti =
					// 		  outcomes.transitions[i].begin();
					// 		ti != outcomes.transitions[i].end(); ti++) {
					// 	     tmp = Cudd_bddAnd(manager, Cudd_Not((*ti)->effect_bdd()), tmp2);
					// 	     Cudd_Ref(tmp);
					// 	     Cudd_RecursiveDeref(manager, tmp2);
					// 	     tmp2 = tmp;
					// 	     Cudd_Ref(tmp2);
					// 	     Cudd_RecursiveDeref(manager, tmp);
					// 	   }
					// 	 }

					//  	 tmp = Cudd_addConst(manager, 1.0 - sum);
					// 	 Cudd_Ref(tmp);
					// 	 tmp1 = Cudd_BddToAdd(manager, tmp2);
					// 	 Cudd_Ref(tmp1);
					// 	 Cudd_RecursiveDeref(manager, tmp2);
					// 	 tmp2 = Cudd_addApply(manager, Cudd_addTimes, tmp, tmp1);
					// 	 Cudd_Ref(tmp2);
					// 	 Cudd_RecursiveDeref(manager, tmp);
					// 	 Cudd_RecursiveDeref(manager, tmp1);

					fr = Cudd_ReadOne(manager);
					Cudd_Ref(fr);
					for (size_t i = 0; i < n; i++) {
						for (TransitionSetList::const_iterator ti =
								outcomes.transitions[i].begin();
								ti != outcomes.transitions[i].end(); ti++) {
							for(int j = 0; j < num_alt_facts; j++){
								tmp1 = Cudd_bddIthVar(manager, (2*j)+1);
								Cudd_Ref(tmp1);
								//	       tmp2 = Cudd_bddAnd(manager, tmp1, (*ti)->effect_bdd());
								//Cudd_Ref(tmp2);
								// 	       std::cout << "ent" << std::endl;
								// 	       printBDD((*ti)->effect_bdd());
								// 	       printBDD(tmp1);
								if(bdd_entailed(manager, (*ti)->effect_bdd(), tmp1)){
									//tmp2 != Cudd_ReadLogicZero(manager)){
									DdNode *tmp4 = Cudd_Not(tmp1);
									Cudd_Ref(tmp4);
									DdNode *tmp3 = Cudd_bddAnd(manager, fr, tmp4);
									Cudd_Ref(tmp3);
									Cudd_RecursiveDeref(manager, tmp4);
									Cudd_RecursiveDeref(manager, fr);
									fr = tmp3;
									Cudd_Ref(fr);
									Cudd_RecursiveDeref(manager, tmp3);
									// std::cout << "fr =" <<std::endl;
									//printBDD(fr);
								}

								Cudd_RecursiveDeref(manager, tmp1);
								//Cudd_RecursiveDeref(manager, tmp2);
							}
						}
					}
					tmp = Cudd_addConst(manager, 1.0 - sum);
					Cudd_Ref(tmp);
					tmp1 = Cudd_BddToAdd(manager, fr);
					Cudd_Ref(tmp1);
					Cudd_RecursiveDeref(manager, fr);
					tmp2 = Cudd_addApply(manager, Cudd_addTimes, tmp, tmp1);
					Cudd_Ref(tmp2);
					Cudd_RecursiveDeref(manager, tmp);
					Cudd_RecursiveDeref(manager, tmp1);


					preffect->push_back(tmp2);
					//	 	 printBDD(tmp2);

					tmp1 = Cudd_Support(manager, tmp2);
					Cudd_Ref(tmp1);
					fr = Cudd_bddAnd(manager, pr_atoms, tmp1);
					Cudd_Ref(fr);
					Cudd_RecursiveDeref(manager, pr_atoms);
					pr_atoms = fr;
					Cudd_Ref(pr_atoms);
					Cudd_RecursiveDeref(manager, fr);

				}


				//       make_mutex(preffect);
				//       make_mutex1(preffect, pr_atoms);
				free_outcomes(outcomes);
			}
		}

		DdNode *tmpDan = iterative_assemble_init(&preffects,
				preffects.begin(),
				b_initial_state,
				Cudd_ReadOne(manager));
		Cudd_Ref(tmpDan);


		//   tmp =  recurse_assemble_init(&preffects,
		// 			       preffects.begin(),
		// 			       b_initial_state,
		// 			       Cudd_ReadOne(manager));
		//   Cudd_Ref(tmp);
		Cudd_RecursiveDeref(manager, b_initial_state);
		b_initial_state = tmpDan;
		Cudd_Ref(b_initial_state);
		Cudd_RecursiveDeref(manager, tmpDan);

		//    std::cout << "HI" <<std::endl;


		//const Atom* a;
		for(int i = 0; i < num_alt_facts; i+=1){
			a = (*(dynamic_atoms.find(i))).second;
			fr=Cudd_Support(manager, b_initial_state);
			Cudd_Ref(fr);
			if(((problem)->init_atoms().find(a)) != (problem)->init_atoms().end()){
				tmp = Cudd_addIthVar(manager, (2*i)+1);
				Cudd_Ref(tmp);
				tmp1 = Cudd_addApply(manager,
						Cudd_addTimes,
						b_initial_state,
						tmp);
				Cudd_Ref(tmp1);
				Cudd_RecursiveDeref(manager, tmp);
				Cudd_RecursiveDeref(manager, b_initial_state);
				b_initial_state = tmp1;
				Cudd_Ref(b_initial_state);
				Cudd_RecursiveDeref(manager, tmp1);
			}
			else if(a &&
					!Cudd_bddIsVarEssential(manager,
							fr,
							(2*i)+1,
							1)
							&&
							!Cudd_bddIsVarEssential(manager,
									pr_atoms,
									(2*i)+1,
									1) ){
				tmp = Cudd_BddToAdd(manager,
						Cudd_Not(Cudd_bddIthVar(manager, (2*i)+1)));
				Cudd_Ref(tmp);
				tmp1 = Cudd_addApply(manager,
						Cudd_addTimes,
						b_initial_state,
						tmp);
				Cudd_Ref(tmp1);
				Cudd_RecursiveDeref(manager, tmp);
				Cudd_RecursiveDeref(manager, b_initial_state);
				b_initial_state = tmp1;
				Cudd_Ref(b_initial_state);
				Cudd_RecursiveDeref(manager, tmp1);
			}
			Cudd_RecursiveDeref(manager,fr);
		}
		Cudd_RecursiveDeref(manager, pr_atoms);
		//cout << "sfdxhi prog"<<endl;
		// if(  Cudd_DebugCheck(manager)){ cout << "problem " << endl; exit(0);   }

		//  cout << "set varmap" <<endl;
		tmp1 = Cudd_addPermute(manager, b_initial_state, varmap);

		Cudd_Ref(tmp1);
		Cudd_RecursiveDeref(manager, b_initial_state);
		b_initial_state = tmp1;
		Cudd_Ref(b_initial_state);
		Cudd_RecursiveDeref(manager, tmp1);

		if(problem->domain().requirements.non_deterministic){
			tmp1 = Cudd_addBddStrictThreshold(manager, b_initial_state, 0.0);
			Cudd_Ref(tmp1);
			Cudd_RecursiveDeref(manager, b_initial_state);
			b_initial_state = tmp1;
			Cudd_Ref(b_initial_state);
			Cudd_RecursiveDeref(manager, tmp1);

		}
	}
	else{
		ConjunctiveEffect* init_effect = new ConjunctiveEffect();
		EffectList& efl = (EffectList&)problem->init_effects();
		// 添加所有的初始Effect
		for(EffectList::iterator ie= efl.begin(); ie!= efl.end(); ie++){
			init_effect->add_conjunct(**ie);
		}

		// init_effect->print(std::cout, problem->domain().predicates(), problem->domain().functions(), problem->terms());
		// 建立Effect和 dbn_nodes的映射关系
		std::map<const pEffect*, dbn_node*> *pr_nodes = dbn::generate_probabilistic_nodes(init_effect);
		dbn* init_dbn = effect_dbn((const pEffect&)*init_effect, pr_nodes);
		//		init_dbn->add_noops(num_alt_facts);
		
		if(  my_problem->domain().requirements.rewards && DBN_PROGRESSION){
			dbn_node* goal = init_dbn->vars[2*(num_alt_facts-2)+1];
			dbn_node* sink = init_dbn->vars[2*(num_alt_facts-1)+1];

			if(goal->cpt)
			  Cudd_RecursiveDeref(manager, goal->cpt);
			if(sink->cpt)
			  Cudd_RecursiveDeref(manager, sink->cpt);

			DdNode* t1 = Cudd_Not(Cudd_bddIthVar(manager, 2*(num_alt_facts-2)+1));
			Cudd_Ref(t1);
			DdNode* t2 = Cudd_Not(Cudd_bddIthVar(manager, 2*(num_alt_facts-1)+1));
			Cudd_Ref(t2);
			goal->cpt = Cudd_BddToAdd(manager, t1);
			Cudd_Ref(goal->cpt);
			sink->cpt = Cudd_BddToAdd(manager, t2);
			Cudd_Ref(sink->cpt);
			Cudd_RecursiveDeref(manager, t1);
			Cudd_RecursiveDeref(manager, t2);
			//      printBDD(goal->cpt);
			//       printBDD(sink->cpt);

		}
		//    std::cout << "HI" << std::endl;
		//    std::cout << "init_dbn:" <<  *init_dbn << std::endl;

		DdNode *allF = Cudd_ReadOne(manager);
		for(int i = 0; i < num_alt_facts; i++){
			DdNode *tmp = Cudd_bddAnd(manager, allF, Cudd_Not(Cudd_bddIthVar(manager, 2*i)));
			Cudd_Ref(tmp);
			Cudd_RecursiveDeref(manager, allF);
			allF = tmp;
			Cudd_Ref(allF);
			Cudd_RecursiveDeref(manager, tmp);
		}
		DdNode* allFAdd = Cudd_BddToAdd(manager, allF);
		Cudd_Ref(allFAdd);

		b_initial_state = progress(init_dbn, allFAdd);
		Cudd_Ref(b_initial_state);
		Cudd_RecursiveDeref(manager, allFAdd);
		Cudd_RecursiveDeref(manager, allF);
		delete init_dbn;

		//     printBDD(b_initial_state);
	}

}


dbn* action_dbn(const Action& action){


	if(action_dbns.count(&action)>0){
		return action_dbns[&action];
	}
	//      std::cout << "Make action DBN: ";
	//  action.print(std::cout, (*my_problem).terms()); std::cout <<std::endl;
	action.setProbabilityFromExpressions(my_problem); //dan
	std::map<const pEffect*, dbn_node*> *pr_nodes = dbn::generate_probabilistic_nodes(&(action.effect()));

	action_dbns[&action] = effect_dbn(action.effect(), pr_nodes);

	//	action_dbns[&action]->add_noops(num_alt_facts);

	//action_dbns[&action]->apply_rewards_and_sink();
	//   for(int i = 0; i < num_alt_facts; i++){
	//     std::cout << (2*i+1) << std::endl;
	//     printBDD(action_dbns[&action]->vars[2*i+1]->cpt);
	//   }
	//   for(int i = 0; i < action_dbns[&action]->num_aux_vars; i++){
	//     std::cout << (2*num_alt_facts+i) << std::endl;
	//     printBDD(action_dbns[&action]->vars[2*num_alt_facts+i]->cpt);
	//   }
	//action_dbns[&action]->get_dds();
	action_rewards[&action] = action_dbns[&action]->vars[-1]->cpt;
	//action_dbns[&action]->get_reward_dd();

	//    if(action_dbns[&action]->vars[-1]!=NULL)
	//   printBDD(action_dbns[&action]->vars[-1]->cpt);





	std::list<DdNode*>* ddos = NULL;
	std::list<std::map<const pEffect*, DdNode*>*>* ddosc = NULL;
	//   std::map<DdNode*, const ProbabilisticEffect*>* ddosc = NULL;
	int err = 0;
	try{
		if(action.hasObservation()){
			//std::cout << "making obs" << std::endl;

			//	ObservationCpt& cpt = ((ObservationCpt&)((Action&)action).observation_cpt());


			if(OBS_TYPE == OBS_CPT
			){
				ddosc =  observation_cpt_mtbdds(action, *my_problem);
			}
			else{
				ddos = observation_mtbdds(action, *my_problem);
			}

		}
	} catch (std::logic_error& e) {
		std::cout << "caught Exception"<<std::endl;
		//exit(0);
		err = 1;
		//printBDD(dds.first);
	}


	if(!err){
		if(ddos && ddos->size() > 0){//&((Action)action).observation()){
			OBSERVABILITY = OBS_PART;
			action_observations.insert(std::make_pair(&action, ddos));
			ddos = NULL;
		}
		else       if(ddosc && ddosc->size() > 0){//&((Action)action).observation()){
			OBSERVABILITY = OBS_PART;
			action_observations_cpt.insert(std::make_pair(&action, ddosc));
			ddosc = NULL;
		}
	}
	//    std::cout << *action_dbns[&action] << std::endl;
	return action_dbns[&action];
	//    std::cout << "HI" << std::endl;
	//action_dbns[&action]->get_dds();

}


DdNode* groundActionDD(const Action& action){
	//  const Problem& problem, DdNode* ddgp, DdNode* ddng,   DdNode* col_cube){

	std::cout << "lookup: ";
	action.print(std::cout, (*my_problem).terms()); std::cout << std::endl;

	if(0 && DBN_PROGRESSION){
		action_dbn(action);
		return Cudd_ReadLogicZero(manager);
	}
	else{

		if(action_transitions.count(&action) > 0){
			return action_transitions[&action];
		}

		//    cout << "grounding: ";
		//    action.print(cout, (*my_problem).terms()); cout <<endl;

		int err = 0;
		action.setProbabilityFromExpressions(my_problem); //dan

		std::pair<DdNode*, DdNode*> dds;
		std::list<DdNode*>* ddos = NULL;
		try{
			DdNode *a = Cudd_ReadOne(manager), *b = Cudd_ReadOne(manager);
			Cudd_Ref(a);
			Cudd_Ref(b);

			dds = action_mtbdds(action, *my_problem,
					a, b, col_cube, false);
			Cudd_RecursiveDeref(manager, a);
			Cudd_RecursiveDeref(manager, b);



			if(action.hasObservation()){
				ddos = observation_mtbdds(action, *my_problem);
			}
		} catch (std::logic_error& e) {
			///    std::cout << "caught Exception"<<std::endl;
			//exit(0);
			err = 1;
			//printBDD(dds.first);
		}


		if(!err){
			action_transitions.insert(std::make_pair(&action, dds.first));
			action_rewards.insert(std::make_pair(&action, dds.second));

			if(ddos && ddos->size() > 0){//&((Action)action).observation()){
				OBSERVABILITY = OBS_PART;
				action_observations.insert(std::make_pair(&action, ddos));
				ddos = NULL;
			}
			//     cout << action.name()
			// 	 << " cost = " << alt_act_costs[action.id()-1]
			// 	 << endl;
		}
		else{
			//   cout << "ERROR, pruning: ";
			//action.print(cout, (*my_problem).terms()); cout <<endl;
			action_transitions.insert(std::make_pair(&action, Cudd_ReadZero(manager)));
			action_rewards.insert(std::make_pair(&action, Cudd_ReadZero(manager)));
			//     Cudd_RecursiveDeref(manager, dds.first);
			//     Cudd_RecursiveDeref(manager, dds.second);
			//    if(ddos)
			//       delete ddos;

			if(LUGTOTEXT)
				num_alt_effs++;//for dummy unconditional effect index
			return Cudd_ReadZero(manager);
		}

		//  printBDD(dds.first);
		return dds.first;
	}

}

DdNode* groundEventDD(const Action& action){

	//  const Problem& problem, DdNode* ddgp, DdNode* ddng,   DdNode* col_cube){

	//     cout << "lookup: ";
	//      action.print(cout, (*my_problem).terms()); cout <<endl;

	if(event_transitions.count(&action) > 0){
		return event_transitions[&action];
	}

	//cout << "grounding: ";
	//action.print(cout, (*my_problem).terms()); cout <<endl;

	int err = 0;
	action.setProbabilityFromExpressions(my_problem); //dan

	std::pair<DdNode*, DdNode*> dds;
	std::list<DdNode*>* ddos = NULL;
	try{
		DdNode *a = Cudd_ReadOne(manager), *b = Cudd_ReadOne(manager);
		Cudd_Ref(a);
		Cudd_Ref(b);
		dds = action_mtbdds(action, *my_problem,
				a, b, col_cube, true);
		Cudd_RecursiveDeref(manager, a);
		Cudd_RecursiveDeref(manager, b);



		//    if(action.hasObservation()){
		//       ddos = observation_mtbdds(action, *my_problem);
		//     }
	} catch (std::logic_error& e) {
		//    std::cout << "caught Exception"<<std::endl;
		//exit(0);
		err = 1;
		//printBDD(dds.first);
	}

	//  cout << "err = " << err << endl;
	if(!err){
		event_transitions.insert(std::make_pair(&action, dds.first));
		event_rewards.insert(std::make_pair(&action, dds.second));

		//     if(ddos && ddos->size() > 0){//&((Action)action).observation()){
		//       OBSERVABILITY = OBS_PART;
		//       event_observations.insert(std::make_pair(&action, ddos));
		//       ddos = NULL;
		//     }
		//     cout << action.name()
		// 	 << " cost = " << alt_act_costs[action.id()-1]
		// 	 << endl;
	}
	else{
		//   cout << "ERROR, pruning: ";
		//action.print(cout, (*my_problem).terms()); cout <<endl;
		event_transitions.insert(std::make_pair(&action, Cudd_ReadZero(manager)));
		event_rewards.insert(std::make_pair(&action, Cudd_ReadZero(manager)));
		//     Cudd_RecursiveDeref(manager, dds.first);
		//     Cudd_RecursiveDeref(manager, dds.second);
		//    if(ddos)
		//       delete ddos;


		return Cudd_ReadZero(manager);
	}


	return dds.first;
}

void computeDecisiveSets(){
	//mark atoms as static that are not in the decisive set

	std::set<const Atom*> decisive;

	std::map<const Atom*, std::set<const Atom*>* > fpos_dependencies;
	std::map<const Atom*, std::set<const Atom*>* > fneg_dependencies;

	std::map<const Action*, std::set<const Atom*>* > apos_dependencies;
	std::map<const Action*, std::set<const Atom*>* > aneg_dependencies;

	for(std::map<int, const Atom*>::iterator i = dynamic_atoms.begin();
			i != dynamic_atoms.end(); i++){

		//compute literal dependencies

		//compute action dependencies


	}


}


double transform_reward_to_probability(double reward) {
	double pr = ((reward-min_reward)*(1-gDiscount-gEpsilon))/(max_reward-min_reward);
	return pr;

}
double transform_probability_to_reward(double pr) {
	double rew = ((pr*(max_reward-min_reward))/(1-gDiscount-gEpsilon)) + min_reward;
	return rew;

}

/* ====================================================================== */
/* solve_problem */

/* Solves the given problem. */
DdNode* solve_problem(const Problem& problem,
		double gamma, double epsilon) {

	gDiscount = (*my_problem).discount();
	//       std::cout << "DISCOUNT = " << gDiscount << std::endl;

	/*
	 * Extract the reward function.
	 */
	std::pair<Function, bool> rf =
			problem.domain().functions().find_function("reward");
	if (rf.second) {
		reward_function = rf.first;
	} else {
		reward_function = problem.domain().functions().last_function() + 1;
	}
	valid_reward_function = rf.second;



	/*
	 * Collect state variables and assign indices to them.
	 */
	for (ActionList::const_iterator ai = problem.actions().begin();
			ai != problem.actions().end(); ai++) {

		const Action& action = **ai;

		std::set<Atom*> pos;
		std::set<Atom*> neg;
		if(LUGTOTEXT && !check_consistent(action.effect(), &pos, &neg)){
			continue;
		}
		collect_state_variables(action.precondition(), false);
		collect_state_variables(action.effect());

		if(&((Action*)(*ai))->observation()){
			SENSORS=TRUE;
			ObservationVector& ob =
					(ObservationVector&) ((Observation&)((Action*)(*ai))->observation()).obVector();
			ObservationEntry *e;
			for(ObservationVector::iterator i = ob.begin();i != ob.end(); i++){
				e = ((ObservationEntry*)(*i));
				//cout << "collect"<<endl;
				collect_state_variables(e->formula(), false);
				if(&(e->symbol()) != NULL)
					collect_state_variables(e->symbol(), false);
			}
		}
	}

	for (ActionList::const_iterator ai = problem.events().begin();
			ai != problem.events().end(); ai++) {
		const Action& action = **ai;
		collect_state_variables(action.precondition(), false);
		collect_state_variables(action.effect());

		if(&((Action*)(*ai))->observation()){
			ObservationVector& ob =
					(ObservationVector&)((Observation&)((Action*)(*ai))->observation()).obVector();
			ObservationEntry *e;
			for(ObservationVector::iterator i = ob.begin();i != ob.end(); i++){
				e = ((ObservationEntry*)(*i));
				//cout << "collect"<<endl;
				collect_state_variables(e->formula(), false);
				// 	 collect_state_variables(e->symbol());
			}
		}
	}

	for (EffectList::const_iterator ei = problem.init_effects().begin();
			ei != problem.init_effects().end(); ei++) {
		//std::cout << "collect init eff" << std::endl;
		collect_state_variables(**ei);


	}
	if(&problem.init_formula()){
		//   std::cout << "collect Init" << std::endl;
		collect_state_variables(problem.init_formula(), true);
	}
	//nvars = state_variables.size(); //dan


	//set Son KR'06
	//computeDescisiveSets();



	nvars = dynamic_atoms.size();


	if(  my_problem->domain().requirements.rewards && DBN_PROGRESSION){
		nvars += 2; //for goal and terminal states
	}


	if (true || verbosity >= 3) {

		std::cout << std::endl << "Number of state variables: " << nvars
				<< std::endl;
		if (true || verbosity >= 3) {
			for (std::map<int, const Atom*>::const_iterator vi =
					dynamic_atoms.begin();
					vi != dynamic_atoms.end(); vi++) {
				std::cout << (*vi).first <<  '\t';
				(*vi).second->print(std::cout, problem.domain().predicates(),
						problem.domain().functions(), problem.terms());
				std::cout << std::endl;
			}
			for (std::map<const Atom*, int>::const_iterator vi =
					state_variables.begin();
					vi != state_variables.end(); vi++) {
				std::cout << (*vi).second <<  '\t';
				(*vi).first->print(std::cout, problem.domain().predicates(),
						problem.domain().functions(), problem.terms());
				std::cout << std::endl;
			}
		}
	}

	if(//my_problem->domain().requirements.rewards &&
			DBN_PROGRESSION){

	  for(ActionList::const_iterator ai = problem.actions().begin(); ai != problem.actions().end(); ai++) {
	    const Action& action = **ai;
	    int num_aux = 0;
	    double a_min_reward = DBL_MAX;
	    double a_max_reward = -1*DBL_MAX;
	    action.effect().getMinMaxRewards(&a_min_reward, &a_max_reward, &num_aux);
	    //std::cout << " num_aux: " << num_aux << std::endl;
	    if(num_aux > max_num_aux_vars){
	      max_num_aux_vars = num_aux;
	    }
	    
	    if(a_max_reward > max_reward)
	      max_reward = a_max_reward;
	    if(a_min_reward < min_reward)
	      min_reward = a_min_reward;
	  }
	  
	  if(min_reward > 0)
	    min_reward = 0;
	  if(max_reward < 0)
	    max_reward = 0;

	  std::cout 	<< "min: " << min_reward << " max: " << max_reward
			<< " discount: " << gDiscount << " epsilon: " << gEpsilon
			<< " num_aux_vars: " << max_num_aux_vars
			<< std::endl;
		//std::cout << transform_reward_to_probability(0) << std::endl;
	}


	/*
	 * Iniiatlize CUDD.
	 */
	int num = (2*nvars)+max_num_aux_vars+(2*rbpf_bits);
	dd_man = Cudd_Init(num, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
	num_alt_facts = nvars;
	manager = dd_man;



// 	  	 	Cudd_AutodynEnable(manager,
// 	// // 	 			   //CUDD_REORDER_SAME
// 	// //  			   //
// CUDD_REORDER_LINEAR
// 	//   			   
// 					   //					   CUDD_REORDER_SIFT
// 					   //					   CUDD_REORDER_WINDOW2
// 					   //			   CUDD_REORDER_WINDOW3
// 	// //  			   //CUDD_REORDER_ANNEALING
// 	// //  			   //CUDD_REORDER_GROUP_SIFT
// 	   			   );


	/*
	 * Collect column variables and compute their cube.
	 */
	DdNode** col_variables = new DdNode*[nvars];
	for (int i = 0; i < nvars; i++) {
		col_variables[i] = Cudd_addIthVar(dd_man, 2*i + 1);
		Cudd_Ref(col_variables[i]);
	}
	col_cube = Cudd_addComputeCube(dd_man, col_variables, NULL, nvars);
	Cudd_Ref(col_cube);
	for (int i = 0; i < nvars; i++) {
		Cudd_RecursiveDeref(dd_man, col_variables[i]);
	}
	delete col_variables;

	/*
	 * Construct identity BDDs for state variables.
	 */
	DdNode** row_vars = new DdNode*[nvars];
	DdNode** col_vars = new DdNode*[nvars];
	for (int i = 0; i < nvars; i++) {
		DdNode* x = Cudd_bddIthVar(dd_man, 2*i);
		DdNode* y = Cudd_bddIthVar(dd_man, 2*i + 1);
		identity_bdds.push_back(Cudd_Xeqy(dd_man, 1, &x, &y));
		Cudd_Ref(identity_bdds.back());
		row_vars[i] = x;
		col_vars[i] = y;
	}
	identity_bdd = Cudd_Xeqy(dd_man, nvars, row_vars, col_vars);
	Cudd_Ref(identity_bdd);
	delete row_vars;
	delete col_vars;


	/*
	 * Construct a BDDs representing goal states.
	 */
	DdNode* ddg;
	if(  my_problem->domain().requirements.rewards && DBN_PROGRESSION){
		ddg = Cudd_bddIthVar(manager, 2*(num_alt_facts-2));
		Cudd_Ref(ddg);
	}
	else{
		ddg= formula_bdd(problem.goal());
	}
	//  cout << "done with goal"<<endl;
	b_goal_state = ddg;
	Cudd_Ref(b_goal_state);

	if (verbosity >= 3) {

		std::cout << std::endl << "Goal state BDD:" << std::endl;
		Cudd_PrintDebug(dd_man, ddg, 2*nvars, 2);
	}
	int* row_to_col = new int[2*nvars];
	for (int i = 0; i < nvars; i++) {
		row_to_col[2*i] = 2*i + 1;
		row_to_col[2*i + 1] = 2*i + 1;
	}
	DdNode* ddgp = Cudd_bddPermute(dd_man, ddg, row_to_col);
	Cudd_Ref(ddgp);
	delete row_to_col;



//Set up variable mappings
	varmap = new int[2*nvars];
	for(int i = 0; i < nvars; i++){
		varmap[2*i] = 2*i+1;
		varmap[2*i+1] = 2*i;
	}

	//   std::cout << "setting action preconds" <<std::endl;

	/*
	 * Construct transition probability and reward MTBDDs for actions.
	 */
	DdNode* ddng = Cudd_Not(ddg);
	Cudd_Ref(ddng);
	Cudd_RecursiveDeref(dd_man, ddg);

	for (ActionList::const_iterator ai = problem.actions().begin();
			ai != problem.actions().end(); ai++) {


		const Action& action = **ai;


		action_preconds.insert(std::make_pair<const Action*, DdNode*>(&action,
				formula_bdd(action.precondition())));
		// printBDD(action_preconds[&action]);
		if(LUGTOTEXT)
			groundActionDD(action);//, problem, ddgp, ddng, col_cube);




	}
	for (ActionList::const_iterator ai = problem.events().begin();
			ai != problem.events().end(); ai++) {
		const Action& action = **ai;
		event_preconds.insert(std::make_pair<const Action*, DdNode*>(&action,
				formula_bdd(action.precondition())));
		//groundActionDD(action);//, problem, ddgp, ddng, col_cube);
	}


	set_cubes();
	//std::cout << "done setting action preconds" <<std::endl;
	collectInit(&problem);
	//std::cout << "done setting action preconds" <<std::endl;

	Cudd_RecursiveDeref(dd_man, ddgp);

	if (verbosity >= 3) {

		for (ActionList::const_iterator ai = problem.actions().begin();
				ai != problem.actions().end(); ai++) {
			const Action& action = **ai;
			std::cout << "Grounding Action: " << action.name() << std::endl;
			groundActionDD(action);
		}
		for (std::map<const Action*, DdNode*>::iterator ai =
				action_transitions.begin();
				ai != action_transitions.end(); ai++) {
			std::cout << std::endl << "Transition probability matrix for ";
			(*ai).first->print(std::cout, problem.terms());
			std::cout << ':' << std::endl;
			Cudd_PrintDebug(dd_man, (*ai).second, 2*nvars, 1);
#if 0
			matrix_to_dbn((*ai).second);
#endif
			std::cout << "Reward vector for ";
			(*ai).first->print(std::cout, problem.terms());
			std::cout << ':' << std::endl;
			Cudd_PrintDebug(dd_man, action_rewards[(*ai).first], 2*nvars, 1);
		}
	}



	DdNode* ddP;// = value_iteration(problem, ddng, col_cube, gamma, epsilon);
	return ddP;
}


/* ====================================================================== */
/* MBDDPlanner */

/* Deletes this MTBDD planner. */
MTBDDPlanner::~MTBDDPlanner() {
	if (mapping_ != NULL) {
		Cudd_RecursiveDeref(dd_man_, mapping_);
		Cudd_DebugCheck(dd_man_);
		int unrel = Cudd_CheckZeroRef(dd_man_);
		if (unrel != 0) {
			std::cerr << unrel << " unreleased DDs" << std::endl;
		}
		Cudd_Quit(dd_man_);
	}
}  


void MTBDDPlanner::initRound() {
	if (mapping_ == NULL) {
		mapping_ = solve_problem(_problem, gamma_, epsilon_);
		dd_man_ = dd_man;
		actions_ = policy_actions;
		policy_actions.clear();
		dynamic_atoms_ = dynamic_atoms;
		dynamic_atoms.clear();
	}
}


const Action* MTBDDPlanner::decideAction(const State& state) {
	DdNode* dds = state_bdd(dd_man_, dynamic_atoms_, state.atoms());
	DdNode* ddS = Cudd_BddToAdd(dd_man_, dds);
	Cudd_Ref(ddS);
	Cudd_RecursiveDeref(dd_man_, dds);
	DdNode* dda = Cudd_addEvalConst(dd_man_, ddS, mapping_);
	Cudd_Ref(dda);
	Cudd_RecursiveDeref(dd_man_, ddS);
	size_t id = int(Cudd_V(dda) + 0.5);
	Cudd_RecursiveDeref(dd_man_, dda);
	std::map<size_t, const Action*>::const_iterator ai = actions_.find(id);
	return (ai != actions_.end()) ? (*ai).second : NULL;
}


void MTBDDPlanner::endRound() {
}