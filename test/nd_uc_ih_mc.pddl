;;non-deterministic
;;uniform cost
;;indefinite horizon
;;minimize cost

(define (domain logistics)
  (:requirements :conditional-effects)
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
;	(in-city ?truck ?city)
 (in-city ?loc ?city)
   (at ?truck ?loc))
  :effect
   (and (when (at ?obj ?loc) 
              (and (not (at ?obj ?loc)) (in ?obj ?truck)))))

(:action LOAD-AIRPLANE
  :parameters
   (?obj
    ?airplane
    ?loc)
  :precondition
   (and (OBJ ?obj) (AIRPLANE ?airplane) (LOCATION ?loc)
    (at ?airplane ?loc)(at ?obj ?loc) )
  :effect
	(and (not (at ?obj ?loc)) (in ?obj ?airplane)))

(:action UNLOAD-TRUCK
  :parameters
   (?obj
    ?truck
    ?loc
    ?city)
  :precondition
   (and (OBJ ?obj) (TRUCK ?truck) (LOCATION ?loc)
;	(in-city ?truck ?city)
 (in-city ?loc ?city)
        (at ?truck ?loc) )
  :effect
  (and 
	(when 
	    (in ?obj ?truck)
	  (and (not (in ?obj ?truck)) (at ?obj ?loc)))))

(:action UNLOAD-AIRPLANE
  :parameters
   (?obj
    ?airplane
    ?loc)
  :precondition
   (and (OBJ ?obj) (AIRPLANE ?airplane) (LOCATION ?loc)
         (at ?airplane ?loc)(in ?obj ?airplane))
  :effect
	 (and (not (in ?obj ?airplane)) (at ?obj ?loc)))

(:action DRIVE-TRUCK
  :parameters
   (?truck
    ?loc-from
    ?loc-to
    ?city)
  :precondition
  (and (TRUCK ?truck) (LOCATION ?loc-from) (LOCATION ?loc-to) (CITY ?city)
;      (in-city ?truck ?city) 
;       (not (= ?loc-from ?loc-to))
       (at ?truck ?loc-from)
       (in-city ?loc-from ?city)
       (in-city ?loc-to ?city))
  :effect
   (and
	(not (at ?truck ?loc-from)) (at ?truck ?loc-to)))

(:action FLY-AIRPLANE
  :parameters
   (?airplane
    ?loc-from
    ?loc-to)
  :precondition
   (and (AIRPLANE ?airplane) (AIRPORT ?loc-from) (AIRPORT ?loc-to)
;       (not (= ?loc-from ?loc-to))
	(at ?airplane ?loc-from))
  :effect
   (and
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
(in-city t0 c0)
(TRUCK t1)
(in-city t1 c1)
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



(oneof
(at p0 l10)
 (at p0 l11)
)

(oneof
 (at p1 l00)
 (at p1 l01)
)


)
(:goal
(and
(at p0 l00)
(at p1 l10)
)

)
)