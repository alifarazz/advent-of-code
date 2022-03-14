import std/heapqueue

# code is not 100 % correct, for k = (1..5)
#   correct: 707 1098 1630 2231 2942
#   mine:    707 1111 1632 2236 2947

const
  N = 100
  M = N
  K = 5


var
  board: array[N, array[M, int]]

proc dijkstraSSP(): int =
  var 
    heap = initHeapQueue[(int, (int, int))]()
    mincost: array[N * K, array[M * K, int]]
    mark: array[N * K, array[M * K, bool]]
    
  block init_data:
    for i in 0..<K * N:
      for j in 0..<K * M:
        mincost[i][j] = int.high
    heap.push((0, (0, 0)))
    mincost[0][0] = 0
    mark[0][0] = true
  
  while heap.len != 0:
    let (cost, ij) = heap.pop
    let (i, j) = ij
    if ij == (K * N - 1, K * M - 1):
      return cost
    
    mark[i][j] = true
    for (oi, oj) in [(0, -1), (0, 1), (1, 0), (1, 0)]:
      let
        ii = i + oi
        jj = j + oj
      if ii >= 0 and ii < K * N and jj >= 0 and jj < K * M and not mark[ii][jj]:
        let mycost = (board[ii mod N][jj mod M] + ii div N + jj div M) mod 9 + 1
        let w = mycost + cost
        if w < mincost[ii][jj]:
          mincost[ii][jj] = w
          heap.push((w, (ii, jj)))
  -1
 
when isMainModule:
  block get_input:
    let input = stdin.readAll
    var k = 0
    for i in 0..<N:
      for j in 0..<M:
        board[i][j] = input[k].ord - '1'.ord
        inc k
      inc k  # jump over newline

#[      
  block output:
    for ii in 0..<K*N:
      for jj in 0..<K*M:
        let mycost = (board[ii mod N][jj mod M] + ii div N + jj div M) mod 9 + 1
        stdout.write mycost
      stdout.write '\n'
]#

  echo dijkstraSSP()
