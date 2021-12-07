from std/sequtils import foldl, mapIt, map
from std/strutils import split, parseInt
from std/algorithm import sorted
from std/stats import mean


func part1(v: openArray[int], median: int): int = v.mapIt(abs(it - median))
                                                   .foldl(a + b)
func part2(v: openArray[int], avg: int): int = v.mapIt(abs(it - avg))
                                                .mapIt(it * (it + 1) div 2)
                                                .foldl(a + b)

when isMainModule:
  # I wish stats.median existed, so I wouldn't have to sort the entire array
  let v = stdin.readAll[0..^2].split(',').map(parseInt).sorted
  let medianIdx =  v.len div 2
  echo min(v.part1(v[medianIdx]), v.part1(v[medianIdx + 1]))
  let avg = int(v.mean) # It's either mean -0.5 or mean + 0.5
  echo min(v.part2(avg), v.part2(avg + 1))
