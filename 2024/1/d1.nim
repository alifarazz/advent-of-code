import std/[strutils, sequtils, algorithm, sugar]


when isMainModule:
  let lines = stdin.readAll()
  let n = lines.count('\n')
  var l = newSeq[int](n)
  var r = newSeq[int](n)

  for idx, line in lines.splitLines()[0..^2]:
    let sp = line.splitWhitespace
    l[idx] = sp[0].parseInt
    r[idx] = sp[1].parseInt
  l.sort()
  r.sort()

  let silver = zip(l, r).mapIt((it[0] - it[1]).abs).foldl(a + b)
  echo silver
