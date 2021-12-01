from std/strutils import parseInt, splitLines
from std/sequtils import map

when isMainModule:
  let v = stdin.readAll.splitLines[0..^2].map(parseInt)
  var c1, c2: int
  inc c1, int(v[0] < v[1])
  inc c1, int(v[1] < v[2])
  for i in 3..<v.len:
    inc c2, int(v[i - 3] < v[i])
    inc c1, int(v[i - 1] < v[i])
  echo c1, '\n', c2
