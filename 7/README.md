``` sh
$ nim r aoc7.nim <input
```
---
#### Hacks
- I've declared the DFS's visited (`seen`) as a global `HashSet`. Is there 
any way to pass a [`SomeSet`](https://nim-lang.org/docs/sets.html#SomeSet) by 
reference and mutate it in the function?
- Not sure about the performance of the way I'm passing `Table`s around between 
functions.
