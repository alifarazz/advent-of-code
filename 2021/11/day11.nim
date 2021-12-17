import std/[sequtils, strutils, algorithm, intsets, deques]

const sz = 10

type OctopusTable = array[sz, array[sz, int]]

const
  p1_steps = 100
  offsets = product([(-1..1).toSeq, (-1..1).toSeq])
                   .filterIt(it[0] != 0 or it[1] != 0)

var table: OctopusTable

func decode(idx: int): (int, int) = (idx div sz, idx mod sz)
func encode(coord: (int, int)): int = coord[0] * sz + coord[1]
func coordExists(coord:(int, int)): bool =
  coord[0] >= 0 and coord[0] < sz and coord[1] >= 0 and coord[1] < sz
func getChildren(coord:(int, int)): seq[(int, int)] =
  offsets.mapIt((it[0] + coord[0], it[1] + coord[1])).filter(coordExists)

proc simulate(table: var OctopusTable): int =
  var
    q: Deque[int]
    seen: PackedSet[int]

  for i in 0..<sz: # step 1
    for j in 0..<sz:
      inc table[i][j]
      if table[i][j] > 9:
        let ijEncode = (i, j).encode
        seen.incl ijEncode
        q.addLast ijEncode

  while q.len != 0: # step 2 bfs
    let coord = q.popFirst().decode
    for child in coord.getChildren:
      inc table[child[0]][child[1]]
      if table[child[0]][child[1]] > 9:
        let childEncoded = child.encode
        if childEncoded notin seen:
          seen.incl childEncoded
          q.addLast childEncoded

  for (i, j) in seen.mapIt(it.decode): # step 3
    table[i][j] = 0

  seen.len

when isMainModule:
  block fillTable:
    var i, j: int
    for c in stdin.readAll:
      if c >= '0' and c <= '9':
        table[i][j] = c.ord - '0'.ord
        inc j
        if j == sz:
          inc i
          j = 0

  var
    acc: int
    p1_table = table
  for i in 0..<p1_steps:
    acc += p1_table.simulate

  var i: int
  while table.mapIt(it.foldl(a + b)).foldl(a + b) != 0:
    discard table.simulate
    inc i

  echo acc, '\n', i
