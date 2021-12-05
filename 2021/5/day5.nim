from std/strscans import scanf
from std/sequtils import mapIt, foldl

const sz = 1024
const diagSz = sz

var table: array[sz * sz, int]
var diagLines: array[diagSz, (int, int, int, int)]

proc drawLine(x1, y1, x2, y2: int) =
  let
    stepx = clamp(x2 - x1, -1, 1)
    stepy = clamp(y2 - y1, -1, 1)
  inc table[x2 * sz + y2]
  var (x1, y1) = (x1, y1)
  while abs(x1 - x2) + abs(y1 - y2) != 0:
    inc table[x1 * sz + y1]
    inc x1, stepx
    inc y1, stepy

when isMainModule:
  var lenDiagLines: int
  for line in stdin.lines:
    var x1, y1, x2, y2: int
    assert scanf(line, ("$i,$i -> $i,$i"), x1, y1, x2, y2)
    if x1 == x2 or y1 == y2:
      drawLine(x1, y1, x2, y2)
    elif abs(x1 - x2) == abs(y1 - y2):
      diagLines[lenDiagLines] = (x1, y1, x2, y2)
      inc lenDiagLines
  echo table.mapIt(int(it > 1)).foldl(a + b)

  for dl in diagLines[0..<lenDiagLines]:
    drawLine(dl[0], dl[1], dl[2], dl[3])
  echo table.mapIt(int(it > 1)).foldl(a + b)
