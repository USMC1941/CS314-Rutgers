;; Given utility functions

;; contains? checks if a symbol x is in list lst
;; This is a top-level check: meaning (contains '(1 2 3) 2) returns #t but (contains '(1 (2 3)) 2) returns #f
;; Inputs:
;;    lst: a list
;;    x: a symbol
;; Output: #t if x is in lst, and #f if x is not in lst
(define contains?
  (lambda (lst x)
    (cond
      ((null? lst) #f)
      ((equal? (car lst) x) #t)
      (else (contains? (cdr lst) x)))))

;; A set is defined to be a list without duplicates
;; unions two sets a and b
;; Inputs: sets a and b
;; Output: a new set a U b
(define union
  (lambda (a b)
    (cond
      ((or (null? a) (null? b)) (append a b))
      ((contains? a (car b)) (union a (cdr b)))
      (else (union (cons (car b) a) (cdr b))))))

;; removeMatch returns a given list lst with all instances of element x removed
;; Input:
;;    lst: a list
;;    x: a symbol
;; Output: a new list with every instance of x removed
;; (if x is not in the list, removeMatch just returns the original list lst)
(define removeMatch
  (lambda (lst x)
    (cond
      ((null? lst) '())
      ((equal? (car lst) x) (removeMatch (cdr lst) x))
      (else (cons (car lst) (removeMatch (cdr lst) x))))))

;; Function for defining epsilon
;; Running (epsilon) just returns "eps"
(define epsilon
  (lambda () "eps"))