from std/sequtils import mapIt, foldl
import std/heapqueue


const
  N = 100
  M = N

var
  board: array[N, array[M, int]]
  mincost: array[N, array[M, int]]
  mark: array[N, array[M, bool]]

proc dijkstraSSP(): int =
  var heap = initHeapQueue[(int, (int, int))]()
  heap.push((0, (0, 0)))

  while heap.len != 0:
    let (cost, ij) = heap.pop
    let (i, j) = ij
    if ij == (N - 1, M - 1):
      return cost
    
    mincost[i][j] = cost
    mark[i][j] = true
    for (oi, oj) in [(0, -1), (0, 1), (1, 0), (1, 0)]:
      let
        ii = i + oi
        jj = j + oj
      if ii >= 0 and ii < N and jj >= 0 and jj < M and not mark[ii][jj]:
        let w = board[ii][jj] + cost
        if w < mincost[ii][jj]:
          mincost[ii][jj] = w
          heap.push((w, (ii, jj)))
  -1

when isMainModule:
  let input = stdin.readAll
  var k = 0
  for i in 0..<N:
    for j in 0..<M:
      board[i][j] = input[k].ord - '0'.ord
      mincost[i][j] = int.high
      inc k
    inc k
    
  echo dijkstraSSP()
