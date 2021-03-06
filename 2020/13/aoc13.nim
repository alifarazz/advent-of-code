import strutils, sequtils, math

func saneMod(n, M: int): int = ((n mod M) + M) mod M

func multInv(a, b: int): int =
  var (a, b) = (a, b)
  let b0 = b
  var x0, x1: int
  x1 = 1
  if b == 1:
    return 1
  while a > 1:
    let q = a div b
    a = a.saneMod b
    swap a, b
    x1 = x1 - q * x0
    swap x0, x1
  if x1 < 0:
    x1 += b0
  return x1

when isMainModule:
  let input = stdin.readAll()[0..^2].splitLines
  let departTime = input[0].parseInt
  let timesStr = input[1].split ","
  let times = timesStr.filterIt(it != "x").map(parseInt)

  block part1:
    let timeMods = times.mapIt(it - departTime.saneMod it)
    let minIdx = timeMods.minIndex
    echo timeMods[minIdx] * times[minIdx]

  block part2:
    var timeMods = newSeq[int](times.len)
    var idx: int
    for i, time in timesStr:
      if time[0] != 'x':
        timeMods[idx] = -i
        inc idx

    # Chinese Remainder Theorem
    let M = times.prod
    proc chrt(y, d: int): int =
      let b = M div d
      b * y * multInv(b, d)
    let x = zip(timeMods, times).mapIt(chrt(it[0], it[1])).sum
    echo x.saneMod M
