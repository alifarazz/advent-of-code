from std/sequtils import toSeq, foldl, mapIt, map
from std/strutils import join, split, splitLines, parseInt
from std/algorithm import reversed, nextPermutation
import std/sets

# Boy do I miss C!
template loop(body: untyped): typed =
  while true:
    body
template until(cond: typed): typed =
  if cond: break

const allSegs = "abcdefg"
const mapping = [
  "abcefg",
  "cf",
  "acdeg",
  "acdfg",
  "bcdf",
  "abdfg",
  "abdefg",
  "acf",
  "abcdefg",
  "abcdfg",
].mapIt(it.items.toSeq().toHashSet)

const lensOf1478 = [1, 4, 7, 8].mapIt(mapping[it].len)

func mapToSegments(segs: string, perm: string): HashSet[char] =
  result = segs.mapIt(allSegs[perm.find(it)]).toHashSet

when isMainModule:
  var p1, p2: int

  for line in stdin.lines:
    let ps = line.split(" | ")
    let
      patterns = ps[0].split()
      signals = ps[1].split()

    p1 += signals.mapIt(int(it.len in lensOf1478)).foldl(a + b)

    var cableArrangement = allSegs
    loop:
      if patterns.mapIt(it.mapToSegments(cableArrangement) in mapping)
                 .foldl(a and b):
        let digits = signals.mapIt(it.mapToSegments(cableArrangement))
                            .mapIt(mapping.find(it))
        p2 += digits.mapIt($it).join().parseInt
        break
      until not cableArrangement.nextPermutation

  echo p1, '\n', p2
