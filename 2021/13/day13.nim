# nimble install itertools
from itertools import takeWhile
import std/[strutils, sequtils, sugar, algorithm]

const sz = 2000

type GridType = array[sz, array[sz, bool]]

func `[]=`[T; U: Ordinal](arr: var GridType; slice: seq[(U, U)];
    value: sink T) {.inline.} =
  for idx in slice:
    arr[idx[0]][idx[1]] = value

func toPair[A](v: openArray[A]): (A, A) = (v[0], v[1])
func reversed[A](t: (A, A)): (A, A) = (t[1], t[0])

func origamiFold(grid: var GridType, fold: (char, int), nx, ny: var int) =
  if fold[0] == 'y':
    nx = fold[1]
    for i in 0..nx:
      let ri = nx * 2 - i
      for j in 0..ny:
        grid[i][j] = grid[i][j] or grid[ri][j]
    dec nx
  elif fold[0] == 'x':
    ny = fold[1]
    for j in 0..ny:
      let rj = ny * 2 - j
      for i in 0..nx:
        grid[i][j] = grid[i][j] or grid[i][rj]
    dec ny

func `$`(grid: GridType, nx, ny: int): string =
  for i in 0..nx:
    for j in 0..ny:
      let c = if grid[i][j] == false:
                ' '
              else:
                '#'
      result = result & c
    result = result & '\n'

when isMainModule:
  let
    v = stdin.readAll[0..^2].splitLines
    strIsEmpty = (a: string) => a.len != 0
    coords = v.takeWhile(strIsEmpty)
              .toSeq()
              .mapIt(it.split(',')
                       .map(parseInt)
                       .toPair()
                       .reversed())
    folds = v.reversed()
             .takeWhile(strIsEmpty)
             .toSeq()
             .mapIt(it.split('='))
             .mapIt((it[0][^1], it[1].parseInt))
             .reversed()

  var grid: GridType
  grid[coords] = true

  let maxs = coords.unzip()
  var
    nx = maxs[0].foldl(max(a, b))
    ny = maxs[1].foldl(max(a, b))

  grid.origamiFold(folds[0], nx, ny)
  echo grid[0..nx].mapIt(it[0..ny].mapIt(it.ord).foldl(a + b)).foldl(a + b)
  for fold in folds:
    grid.origamiFold(fold, nx, ny)
  echo `$`(grid, nx, ny)
