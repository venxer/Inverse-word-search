HOMEWORK 6: INVERSE WORD SEARCH


NAME:  < Edwin Zhao >


COLLABORATORS AND OTHER RESOURCES: 
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

<https://www.geeksforgeeks.org/
https://www.w3schools.com/
https://cplusplus.com/
Florence Wang>

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.

ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 17 >


ALGORITHM ANALYSIS:
What's the order notation of your algorithm?
puzzleGenerator: O((w * h * r * f * l)^8)
generateBoards: O((w * h)^26 * f)
generateReflections: O(s * h * w/2(w * h/2 + f * l))

TEST CASE SUMMARY:
How did your program perform on the different test cases?  Summarize
the running times.  (It's ok if it didn't finish the harder examples.)
What new test cases did you create and how did it perform on those
tests?
puzzle1 ~1.507ms to ~3.093ms
puzzle2 ~2.032ms to ~3.142ms
puzzle3 ~61.542ms to ~66.779ms
puzzle4 ~8.601ms to ~10.896ms
puzzle5 ~84.775ms to ~89.719ms
puzzle6 ~1724.873ms to ~1873.465ms
puzzle7 ~10459.342ms to ~10659.279ms
puzzle8 ~51716.590ms to ~52144.052ms
custom: ~2350.732ms to ~2360.298ms


MISC. COMMENTS TO GRADER:  
puzzleGenerator iterates thorugh each cell (w * h) and at each cell 
tries to place each letter of the word (r * l) which checking that neg words
do not occur (f). It also checks each cardinal direction. (^8)

generateBoards iterates though each cell (w * h) and attempts to place each alphabetical
letter (^26) while checking for neg words (f)

generateReflections iterates though solutions (s). For each solution, Reflect 
horizonally (h/2), reflect vertically (w/2 * h), and rotates each new solution generated 
(s * h * h * w (w * h)). Witin those new solutions, it reverses each row (w * h/2)
It also checks for neg words each time (f * l)

Custom input:
4 4
+ fire
+ iron
+ rain
+ ear
