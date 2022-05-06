(define (domain test)
 (:requirements :probabilistic-effects)
 (:predicates (win) 
              (flip)
	      (nflip)
(a)
 )
 
 (:action look
  :precondition (not (a))
  :effect (and (a))
  :observation ( ((flip) 1) )
 )

 (:action winner
  :precondition (and);(a)
  :effect (and ;(win))
	(when (and (not (nflip)) (flip)) (win))
               (when (and (nflip) (not (flip))) (not (win))))
 )

)

(define (problem test1)
 (:domain test)
 (:objects )
 (:init 
 
 (probabilistic 0.5 (flip) 0.5 (nflip))
 )
 (:goal (win) 0.1)
)