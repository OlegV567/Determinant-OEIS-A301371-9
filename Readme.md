# Determinant-OEIS-A301371-9

The aim is to find maximum determinant of an 9x9 matrix with 9 copies of the numbers 1,2,...,9 (see [OEIS.A301371](https://oeis.org/A301371)).

Starting point is [OEIS.A328030](https://oeis.org/A328030), 
with circulant 9x9 matrices whose rows are permutations of 1,2,..,9, with determinant 929587995.

Main idea of the approach could be find [here](https://math.stackexchange.com/questions/885481/maximum-determinant-of-latin-squares/1966427#1966427). 

More detailed descriprion is in 'PT-2019-11.pdf' file.

The code performs exhaustive search (in specified range) of appropriate matrices.

To find 9x9 matrices with determinant 929587995, one can use file 'settings.txt' with following parameters:
```text
diag_min:       285
diag_max:       285
background_min: 216
background_max: 219
equal_in_row:   1
display_val:    9
cand_max: 1000000000
```
(running time: 4-10 min.)

To find 9x9 matrices with determinant 933251220, one can use file 'settings.txt' with parameters:
```text
diag_min:       285
diag_max:       285
background_min: 217
background_max: 218
equal_in_Row:   2
display_val:    9
cand_max:       1000000000
```
(running time: 8-20 min.)

Other range expandings consume much more time (feel free to try another parameters).


