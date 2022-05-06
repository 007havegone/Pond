(define (domain apr)
(:requirements :probabilistic-effects)
(:predicates (a))
(:action givea
 :parameters ()
 :precondition (not (a))
 :effect (probabilistic 0.5 (a)))
(:action givea1
 :parameters ()
 :precondition (not (a))
 :effect (probabilistic 0.6 (a))) 
)

(define (problem apr1)
 (:domain apr)
 (:init 
  (probabilistic 0.1 (a))
 )
 (:goal (a) 0.9)
)