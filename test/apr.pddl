(define (domain apr)
(:requirements :probabilistic-effects)
(:predicates (a))
(:action givea
 :parameters ()
 :precondition (and)
 :effect (probabilistic 0.5 (a)))

)

(define (problem apr1)
 (:domain apr)
 (:init 
  (probabilistic 0.1 (a))
 )
 (:goal (a) 0.9)
)