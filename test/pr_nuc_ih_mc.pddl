;;probabilistic
;;non-uniform cost
;;indefinite horizon
;;minimize cost

(define (domain logistics)
  (:requirements :conditional-effects :probabilistic-effects :rewards)
  (:predicates 	(OBJ ?obj)
	       	(TRUCK ?truck)
               	(LOCATION ?loc)
		(AIRPLANE ?airplane)
                (CITY ?city)
                (AIRPORT ?airport)
		(at ?obj ?loc)
		(in ?obj1 ?obj2)
		(in-city ?obj ?city))
 
  ; (:types )		; default object

(:action LOAD-TRUCK
  :parameters
   (?obj
    ?truck
    ?loc
    ?city)
  :precondition
   (and (OBJ ?obj) (TRUCK ?truck) (LOCATION ?loc)
        (in-city ?loc ?city)
   (at ?truck ?loc))
  :effect
   (and (decrease (reward) 5)
   (when (at ?obj ?loc) 
              (probabilistic 0.875 (and (not (at ?obj ?loc)) (in ?obj ?truck))))))

(:action LOAD-AIRPLANE
  :parameters
   (?obj
    ?airplane
    ?loc)
  :precondition
   (and (OBJ ?obj) (AIRPLANE ?airplane) (LOCATION ?loc)
    (at ?airplane ?loc) )
  :effect (and (decrease (reward) 15) 
	(when (at ?obj ?loc) 
	 (probabilistic 0.9 (and (not (at ?obj ?loc)) (in ?obj ?airplane))))))

(:action UNLOAD-TRUCK
  :parameters
   (?obj
    ?truck
    ?loc
    ?city)
  :precondition
   (and (OBJ ?obj) (TRUCK ?truck) (LOCATION ?loc)
 (in-city ?loc ?city) (at ?truck ?loc) )
  :effect
  (and (decrease (reward) 5)
	(when (in ?obj ?truck)
	  (probabilistic 0.75 (and (not (in ?obj ?truck)) (at ?obj ?loc))))))

(:action UNLOAD-AIRPLANE
  :parameters
   (?obj
    ?airplane
    ?loc)
  :precondition
   (and (OBJ ?obj) (AIRPLANE ?airplane) (LOCATION ?loc)
         (at ?airplane ?loc))
  :effect (and (decrease (reward) 15)
	 (when (in ?obj ?airplane) 
	 (probabilistic 0.8 (and (not (in ?obj ?airplane)) (at ?obj ?loc))))))

(:action DRIVE-TRUCK
  :parameters
   (?truck
    ?loc-from
    ?loc-to
    ?city)
  :precondition
  (and (TRUCK ?truck) (LOCATION ?loc-from) (LOCATION ?loc-to) (CITY ?city)
       (at ?truck ?loc-from)
       (in-city ?loc-from ?city)
       (in-city ?loc-to ?city))
  :effect
   (and (decrease (reward) 5)
	(not (at ?truck ?loc-from)) (at ?truck ?loc-to)))

(:action FLY-AIRPLANE
  :parameters
   (?airplane
    ?loc-from
    ?loc-to)
  :precondition
   (and (AIRPLANE ?airplane) (AIRPORT ?loc-from) (AIRPORT ?loc-to)
	(at ?airplane ?loc-from))
  :effect
   (and (decrease (reward) 15)
	(not (at ?airplane ?loc-from)) (at ?airplane ?loc-to)))
)


(define (problem logistics-c2-s2-p2-a2)
(:domain logistics)
(:objects a0 
          c0 c1 
          t0 t1 
          l00 l01 l10 l11 
          p0 p1 
)

(:init
(AIRPLANE a0)
(CITY c0)
(CITY c1)
(TRUCK t0)
(TRUCK t1)
(LOCATION l00)
(in-city  l00 c0)
(LOCATION l01)
(in-city  l01 c0)
(LOCATION l10)
(in-city  l10 c1)
(LOCATION l11)
(in-city  l11 c1)
(AIRPORT l00)
(AIRPORT l10)
(OBJ p0)
(OBJ p1)

(at t0 l00)
(at t1 l10)
(at a0 l10)



(probabilistic
0.5 (at p0 l10)
0.5  (at p0 l11)
)

(probabilistic
 0.5 (at p1 l00)
 0.5 (at p1 l01)
)
)
(:goal
(and
(at p0 l00)
(at p1 l10)
)
0.1296

)
(:goal-reward 0)
)