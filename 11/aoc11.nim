import sugar, strutils, sequtils, math

const displacements = [(-1, -1), (-1, 0), (-1, 1),
                       (0, -1), (0, 1),
                       (1, -1), (1, 0), (1, 1)]

type Map = seq[string]

func `$`(m: Map): string = m.join("\n")
func existsAt(m: Map, i, j: int): bool = i in 0 ..< len(m) and j in 0 ..< len(m[0])
func countCell(m: Map, cell: char): int =
  m.map(x => x.map(y => ord(y == cell)).sum()).sum()

func rayIntersect(m: Map, x, y, dx, dy: int): bool =
  var
    x = x + dx
    y = y + dy
    done = false
  if dx == 0 and dy == 0:
    result = m.existsAt(x, y) and m[x][y] == '#'
    done = true
  while m.existsAt(x, y) and not done:
    if m[x][y] == '#':
      result = true
      done = true
    elif m[x][y] == 'L':
      done = true
    else:
      x += dx
      y += dy

func simulate(m: Map, part1: bool): Map =
  result = m
  for i in 0 ..< len(m):
    for j in 0 ..< len(m[0]):
      if m[i][j] == '.':
        continue
      var adj_cells = 0
      for (di, dj) in displacements:
        if part1:
          adj_cells += ord(m.rayIntersect(i + di, j + dj, 0, 0))
        else:
          adj_cells += ord(m.rayIntersect(i, j, di, dj))
      if m[i][j] == '#' and adj_cells >= (ord(not part1) + 4):
        result[i][j] = 'L' #  c++ like references in Nim?
      elif m[i][j] == 'L' and adj_cells == 0:
        result[i][j] = '#' #  c++ like references in Nim?

func solve(m: Map, part1: bool): Map =
  var m = m
  result = m.simulate(part1)
  while result != m:
    m = result
    result = m.simulate(part1)

when isMainModule:
  let m = stdin.readAll()[0 .. ^2].splitLines()
  echo m.solve(true).countCell('#')
  echo m.solve(false).countCell('#')
