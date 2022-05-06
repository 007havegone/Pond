(define (domain rockSample)
 (:requirements :probabilistic-effects
                :negative-preconditions
                :conditional-effects
                :rewards
                :typing)
 (:types
    pos
    rock
    dist
 )
 (:constants d0 d1 d2 d3 d4 d5 d6 d7 - dist
 )
 (:predicates
   (pos_x ?x - pos)
   (pos_y ?y - pos)
   (succ ?p ?pn - pos)
   (good ?r -rock)
   (rpos_x ?r - rock ?x - pos)
   (rpos_y ?r - rock ?y - pos)
   (exit)
   (done)
   (sampled ?r - rock)
   (exitBoundary ?x - pos)
   (distance ?x ?xn - pos ?d - dist)
 )

 (:action north
  :parameters (?x ?y ?yn - pos)
  :precondition (and (not (exit)) (not (done)) (pos_x ?x) (pos_y ?y) (succ ?y ?yn))
  :effect (and (not (pos_y ?y)) (pos_y ?yn) (decrease (reward) 1))
 )
 (:action south
  :parameters (?x ?y ?yn - pos)
  :precondition (and (not (exit)) (not (done)) (pos_x ?x) (pos_y ?y) (succ ?yn ?y))
  :effect (and (not (pos_y ?y)) (pos_y ?yn) (decrease (reward) 1))
 )
 (:action east
  :parameters (?x ?y ?xn - pos)
  :precondition (and (not (exit)) (not (done)) (pos_x ?x) (pos_y ?y) (succ ?x ?xn))
  :effect (and (not (pos_x ?x)) (pos_x ?xn) (decrease (reward) 1))
 )

 (:action exit
  :parameters (?x - pos)
  :precondition (and (not (exit)) (pos_x ?x) (exitBoundary ?x))
  :effect (and (exit))
 )

 (:action west
  :parameters (?x ?y ?xn - pos)
  :precondition (and (not (exit)) (not (done)) (pos_x ?x) (pos_y ?y) (succ ?xn ?x))
  :effect (and (not (pos_x ?x)) (pos_x ?xn) (decrease (reward) 1))
 )


 (:action sample
  :parameters (?x ?y - pos ?r - rock)
  :precondition (and (not (exit)) (not (done)) (pos_x ?x) (pos_y ?y) (rpos_x ?r ?x) (rpos_y ?r ?y) (not (sampled ?r)))
  :effect (and
      (sampled ?r)
		(when (good ?r) (increase (reward) 10))
		(when (not (good ?r)) (decrease (reward) 10))
	)
 )


(:action end
 :precondition (not (done))
 :effect (and 
		(when (exit) (done))
  )
 )

(:action check_0_0
 :parameters (?x ?y ?rx ?ry - pos ?r - rock)
 :precondition (and (pos_x ?x) (pos_y ?y) 
		    (rpos_x ?r ?x) (rpos_y ?r ?y) 
                    (distance ?x ?rx d0)
                    (distance ?y ?ry d0)
		)
 :observation (((good ?r) 1 1))
 )

(:action check_0_1
 :parameters (?x ?y ?rx ?ry - pos ?r - rock)
 :precondition (and (pos_x ?x) (pos_y ?y) 
		    (rpos_x ?r ?x) (rpos_y ?r ?y) 
                    (distance ?x ?rx d0)
                    (distance ?y ?ry d1)
		)
 :observation (((good ?r) 0.965936 0.965936))
 )

(:action check_1_0
 :parameters (?x ?y ?rx ?ry - pos ?r - rock)
 :precondition (and (pos_x ?x) (pos_y ?y) 
		    (rpos_x ?r ?x) (rpos_y ?r ?y) 
                    (distance ?x ?rx d1)
                    (distance ?y ?ry d0)
		)
 :observation (((good ?r) 0.965936 0.965936))
 )

(:action check_1_1
 :parameters (?x ?y ?rx ?ry - pos ?r - rock)
 :precondition (and (pos_x ?x) (pos_y ?y) 
		    (rpos_x ?r ?x) (rpos_y ?r ?y) 
                    (distance ?x ?rx d1)
                    (distance ?y ?ry d1)
		)
 :observation (((good ?r) 0.952169 0.952169))
 )
)

