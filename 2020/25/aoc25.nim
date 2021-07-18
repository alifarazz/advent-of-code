import strutils

const n = 20201227

proc powmod(base, power, m: int): int =
  var p = power
  result = 1
  while p > 0:
    if p mod 2 == 1:
      result = (result * base) mod m
      dec p
    else:
      result = (result * result) mod m
      p = p shr 1

when isMainModule:
  let p1 = stdin.readLine.parseInt
  let p2 = stdin.readLine.parseInt
  for i in 0..<n:
    if powmod(7, i, n) == p1:
      echo powmod(p2, i, n)
      break
