from std/strutils import parseInt, splitLines
from std/sequtils import map

when isMainModule:
  let v = stdin.readAll.splitLines[0..^2].map(parseInt)
  var c1, c2: int
  for i in 1..<v.len:
    if v[i - 1] < v[i]:
      inc c1
  for i in 3..<v.len:
    if v[i - 3] + v[i - 2] + v[i - 1] < v[i - 2] + v[i - 1] + v[i]:
      inc c2
  echo c1, '\n', c2
