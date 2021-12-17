import std/[sequtils, strutils, algorithm, intsets]

const neighOffsets = [(-1, 0), (0, -1), (0, 1), (1, 0)]

func coordExists(coord:(int, int), nrow, ncol: int): bool =
  coord[0] >= 0 and coord[0] < nrow and coord[1] >= 0 and coord[1] < ncol

func pointIsLow(hmap: openArray[string], coord: (int, int), nrow, ncol: int): bool =
  let value = hmap[coord[0]][coord[1]]
  neighOffsets.mapIt((it[0] + coord[0], it[1] + coord[1]))
              .filterIt(it.coordExists(nrow, ncol))
              .mapIt(value < hmap[it[0]][it[1]])
              .foldl(a and b)

proc floodFill(basin: (int, int), hmap: seq[string], nrow, ncol: int): int =
  var seen: PackedSet[int]

  proc stillIsInBasin(coordMain, coordNext: (int, int)): bool =
    let nxt = hmap[coordNext[0]][coordNext[1]]
    result = nxt < '9' and nxt > hmap[coordMain[0]][coordMain[1]]

  proc dfs(idx: int) =
    seen.incl idx
    let
      coord = (idx div ncol, idx mod ncol)
      children = neighOffsets.mapIt((it[0] + coord[0], it[1] + coord[1]))
                             .filterIt(it.coordExists(nrow, ncol))
                             .filterIt(stillIsInBasin(coord, it))
                             .mapIt(it[0] * ncol + it[1])
                             .filterIt(it notin seen)
    for child in children: child.dfs

  let idx = basin[0] * ncol + basin[1]
  idx.dfs
  result = seen.len

when isMainModule:
  let
    v = stdin.readAll[0..^2].splitLines
    nrow = v.len
    ncol = v[0].len
  var basins: seq[(int, int)]

  for i in 0..<nrow:
    for j in 0..<ncol:
      if v.pointIsLow((i, j), nrow, ncol):
        basins.add((i, j))

  echo basins.mapIt(v[it[0]][it[1]].ord - '0'.ord + 1).foldl(a + b), '\n'
       basins.mapIt(it.floodFill(v, nrow, ncol)).sorted()[^3..^1].foldl(a * b)
