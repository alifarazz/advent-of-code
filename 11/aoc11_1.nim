import sugar, strutils, sequtils, strformat, algorithm, math

type Map = seq[string]

func `$`(m: Map): string = m.join("\n")
func idxExists(m: Map, i, j: int): bool =
  i in 0 ..< len(m) and j in 0 ..< len(m[0])
func countCell(m: Map, cell: char): int =
  m.map(x => x.map(y => ord(y == cell)).sum()).sum()

proc simulate(m: Map): Map =
  result = m
  for i in 0 ..< len(m):
    for j in 0 ..< len(m[i]):
      if m[i][j] == '.':
        continue
      var numberOfOccupiedAround = 0
      for di in -1 .. 1:
        for dj in -1 .. 1:
          let idx_i = i + di
          let idx_j = j + dj
          if i == 0 and j == 2 and m.idxExists(idx_i, idx_j):
            echo idx_i, "\t", idx_j, "\t", m[idx_i][idx_j]
          if not(di == 0 and dj == 0) and m.idxExists(idx_i, idx_j) and
             m[idx_i][idx_j] == '#':
            inc numberOfOccupiedAround
      if i == 0 and j == 2:
        echo numberOfOccupiedAround
      if m[i][j] == '#' and numberOfOccupiedAround >= 4:
        result[i][j] = 'L' #  c++ like references in Nim?
      elif m[i][j] == 'L' and numberOfOccupiedAround == 0:
        result[i][j] = '#' #  c++ like references in Nim?


when isMainModule:
  var m = stdin.readAll()[0 .. ^2].splitLines()
  echo m
  echo "--------------------------"

  var stop = false
  while not stop:
    var m2 = m.simulate()
    echo m2
    echo "--------------------------"
    if m == m2:
      stop = true
    else:
      m = m2
