import std/[strutils, sequtils, algorithm, tables]

when isMainModule:
  let lines = stdin.readAll()[0..^2]
  let n = lines.count('\n') + 1
  var l = newSeq[int](n)
  var r = newSeq[int](n)

  for idx, line in lines.splitLines()[0..^1]:
    let sp = line.splitWhitespace
    l[idx] = sp[0].parseInt
    r[idx] = sp[1].parseInt

  let rcnttable = newCountTable(r)
  let gold = l.mapIt(it * rcnttable[it]).foldl(a + b)

  l.sort()
  r.sort()
  let silver = zip(l, r).mapIt((it[0] - it[1]).abs).foldl(a + b)

  echo silver
  echo gold
