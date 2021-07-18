``` sh
$ nim r aoc8.nim <input
```
---
Running time order (n = total instructions in `input`, m = number of `jmp` and `nop` instructions):
- Part1: BigOh(n)
- Part2: ~~BigOh(n * m)~~ BigOh(n)
#### Hacks
- In part 2, ~~running the machine for m times isn't probably the most efficient solution. ever.~~ O(N) by the power of Dynamic Programming!
You only change one instruction but all others are the same. Any instruction you see after the one you've changed, except for the one corrupted instruction, will not terminate. In case it terminates, then you've found the corrupted instruction!
