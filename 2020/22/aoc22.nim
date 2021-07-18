import strutils, sequtils, math, sets, deques

var winner: Deque[int]

func toString*[T](x: Deque[T]): string = x.mapIt($it).join

proc norec(p1, p2: seq[int]): bool =
  var
    p1 = p1.toDeque
    p2 = p2.toDeque

  while p1.len > 0 and p2.len > 0:
    let
      a = p1.popFirst
      b = p2.popFirst
    if a > b:
      p1.addLast a
      p1.addLast b
    else:
      p2.addLast b
      p2.addLast a
  result = p1.len == 0
  winner = (if result: p2 else: p1)

proc rec(p1, p2: seq[int], update: bool = true): bool =
  var
    p1 = p1.toDeque
    p2 = p2.toDeque
  var seen: HashSet[string]
  while p1.len > 0 and p2.len > 0:
    let k = p1.toString & "|" & p2.toString
    if k in seen:
      return true
    seen.incl k

    let
      a = p1.popFirst
      b = p2.popFirst
    let p1Wins =
      (if a <= p1.len and b <= p2.len:
        rec(toSeq(p1)[0..a-1], toSeq(p2)[0..b-1], false)
      else:
        a > b)
    if p1Wins:
      p1.addLast a
      p1.addLast b
    else:
      p2.addLast b
      p2.addLast a
  result = p1.len > 0
  if update:
    winner = if result: p1 else: p2

when isMainModule:
  let input = stdin.readAll[0..^2].split "\n\n"
  var p1 = input[0].splitlines[1..^1].map(parseInt)
  var p2 = input[1].splitlines[1..^1].map(parseInt)

  proc calcScore(x: seq[int]): int =
    x.zip(toSeq(countDown(x.len, 1))).mapIt(it[0] * it[1]).sum

  discard norec(p1, p2)
  echo toSeq(winner).calcScore

  discard rec(p1, p2)
  echo toSeq(winner).calcScore
