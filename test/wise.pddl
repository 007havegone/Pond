(define (domain wise_domain)
  (:requirements :probabilistic-effects)

  (:predicates
    (percieved_as_wise)
    (percieved_as_boring)
  )

  (:action share_wisdom
    :effect (and (probabilistic 0.8 (percieved_as_wise)) (probabilistic 0.3 (percieved_as_boring)))
    :observation (
      ((percieved_as_wise) 0.8 0.8)
      ((percieved_as_boring) 0.8 0.8)
    )
  )

  (:action tell_a_joke
    :effect (and (probabilistic 0.9 (not (percieved_as_boring))))
    :observation (
      ((percieved_as_boring) 0.8 0.8)
    )
  )
)
(define (problem wise_prob)
  (:domain wise_domain)
  (:requirements :negative-preconditions)
  (:goal
    (and 
      (percieved_as_wise)
      (not (percieved_as_boring))
    )
  )
)
