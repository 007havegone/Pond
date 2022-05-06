(define (domain tiger)
 (:requirements :probabilistic-effects :conditional-effects :fluents :rewards)
 (:predicates (left))
 (:observations (hear_left) )

 (:action listen
  :precondition (and)
  :effect (and (decrease (reward) 1))
  :observation 
   (and  
      (when (left) (probabilistic .85 (hear_left) .15 (not (hear_left))))
      (when (not (left))(probabilistic .15 (hear_left) .85 (not (hear_left))))))

 (:action open_left
  :precondition (and)
  :effect (and (when (not (left)) (increase (reward) 10))
               (when (left)(decrease (reward) 100))
	       (probabilistic 0.5 (left) 0.5 (not (left))))
  :observation 
   (and  
      (when (left) (probabilistic .5 (hear_left) .5 (not (hear_left))))
      (when (not (left))(probabilistic .5 (hear_left) .5 (not (hear_left))))))

 (:action open_right
  :precondition (and)
  :effect (and (when (not (left)) (decrease (reward) 100))
               (when (left) (increase (reward) 10))
	       (probabilistic 0.5 (left) 0.5 (not (left))))
  :observation 
   (and  
      (when (left) (probabilistic .5 (hear_left) .5 (not (hear_left))))
      (when (not (left))(probabilistic .5 (hear_left) .5 (not (hear_left))))))
)

(define (problem tiger1)
 (:domain tiger)
 (:requirements :probabilistic-effects)
 (:init
	(probabilistic 0.5 (left))
 )
 (:metric maximize (reward))
 ;; (:horizon 100)
 ;; (:discount 1.0)
)