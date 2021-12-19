import std/[strutils, sequtils, strtabs, tables]

const
  stepCount1 = 10
  stepCount2 = 40

func synthesize(polyBag: CountTable[string],
                polyTemplates: StringTableRef): CountTable[string] =
  var newPolyBag = polyBag
  for (k, v) in polyBag.pairs:
    let p = polyTemplates.getOrDefault(k)
    if p.len > 0:
      newPolyBag.inc(k, -v)
      let newK1 = k[0] & p
      let newK2 = p & k[1]
      newPolyBag.inc(newK1, v)
      newPolyBag.inc(newK2, v)
  newPolyBag

func calcScore(polyBag: CountTable[string], head, tail: char): int =
  var ct = toCountTable(head & tail)
  for (k, v) in polyBag.pairs:
    ct.inc(k[0], v)
    ct.inc(k[1], v)
  result = ct.largest[1] - ct.smallest[1]
  result = result div 2

when isMainModule:
  let
    v = stdin.readAll[0..^2].splitLines
    (head, tail) = (v[0][0], v[0][^1])
    polyTemplates = v[2..^1].mapIt(it.split(" -> "))
                            .mapIt(((it[0]), it[1]))
                            .newStringTable
  var polyBag = v[0].zip(v[0][1..^1])
                    .mapIt(it[0] & it[1])
                    .toCountTable

  for i in 0..<stepCount1:
    polybag = polybag.synthesize(polytemplates)
  echo polybag.calcScore(head, tail)

  for i in stepCount1..<stepCount2:
    polybag = polybag.synthesize(polytemplates)
  echo polybag.calcScore(head, tail)
