(define (domain apr)
(:requirements :probabilistic-effects)
(:predicates (a))
(:action givea
 :parameters ()
 :precondition (and)
 :effect (probabilistic 0.5 (a)))

)
