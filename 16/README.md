``` sh
$ nimble install neo
$ nim r -d:nimWorkaround14447 aoc16.nim <input
```
---
- I used the [neo](https://github.com/unicredit/neo) matrix library because I grew tired of nesting `seq`s.
- The matching part of second part can be done much more efficiently using [Hopcroft-Karp](https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm) algorithm for bigger inputs.
- ~~The are some problems with passing around `array` data types. Probably due to the fact that `lent[T]` is still buggy (hence the workaround switch).~~ Yep, `sink` and `lent` are buggy. Switch out `.filter(x => stuff(x))` for `filterIt(stuff(it))` also helped.
