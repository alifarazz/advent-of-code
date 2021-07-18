``` sh
$ nim r aoc13.nim <input
```
---
Part 2 using [Chinese Remainder Theorem](https://crypto.stanford.edu/pbc/notes/numbertheory/crt.html)

UPDATE: Now that I paid more attention to the input, I see that the numbers are all primes. So one can solve part 2 without using CRT. Simply iterate over the numbers and increase t (the answer) by the lcm of all previously iterated numbers until you get the reaminder of t (mod the current number) which you want, then goto the next number.

