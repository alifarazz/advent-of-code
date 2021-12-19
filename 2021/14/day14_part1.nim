import std/[strutils, sequtils, sugar, algorithm, strtabs, tables]

const stepCount1 = 10

when isMainModule:
  let v = stdin.readAll[0..^2].splitLines
  let polyTemplates = v[2..^1].mapIt(it.split(" -> "))
                              .mapIt(((it[0]), it[1]))
                              .newStringTable
  var polymer = v[0].toSeq
  polymer.add '\0'

  for i in 0..<stepCount1:
    var j: int
    while polymer[j + 1] != '\0':
      let pair = polymer[j] & polymer[j + 1]
      let newC = polyTemplates.getOrDefault(pair)
      if newC.len > 0:
        inc j
        polymer.insert(newC, j)
      if j == 10:
        quit(-1)
      inc j
  let freqs = polymer[0..^2].toCountTable().values.toSeq()
  echo freqs.max - freqs.min
