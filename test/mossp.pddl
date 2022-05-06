(define (domain mossp_test)
	(:requirements :probabilistic-effects)
        (:types o a b)
	(:predicates (var ?x - o) (vara ?x - a) (varb ?x - b)
	)
	(:action set_a
	 :parameters (?x - o ?y - a)
	 :precondition (and (var ?x) (not (vara ?y)))
	 :effect (probabilistic 1.0 (vara ?y)))
	(:action set_b
	 :parameters (?x - a ?y - b)
	 :precondition (and (vara ?x) (not (varb ?y)))
	 :effect (probabilistic 0.5 (varb ?y)))
	(:action set_b_bad
	 :parameters (?x - a ?y - b)
	 :precondition (and (vara ?x) (not (varb ?y)))
	 :effect (probabilistic 0.65 (and (not (vara ?x)) (varb ?y))))
)


(define (problem mossp_test1)
	(:domain mossp_test)
	(:objects c - o d - a e f - b)
	(:init (var c)
         )
	(:goal (and  (varb e) (varb f)
	))
)