# unwashed ass
import std/strutils

proc main() =
  let lines = stdin.readAll[0..^2].split()
  let n = lines[0].len

  var silver = 4 * (n - 1)
  for i in 1..n - 2:
    for j in 1..n - 2:
      var t, d, r, l : char
      for k in 0..<j: l = max(l, lines[i][k])
      for k in j+1..<n: r = max(r, lines[i][k])
      for k in 0..<i: t = max(t, lines[k][j])
      for k in i+1..<n: d = max(d, lines[k][j])
      let me = lines[i][j]
      silver += ((me > d) or (me > t) or (me > l) or (me > r)).int
  echo silver

  var gold = 0
  for i in 1..n - 2:
    for j in 1..n - 2:
      var t, d, r, l = 0
      let me = lines[i][j]
      while j - l - 1 >= 0 and lines[i][j - l - 1] < me: inc l
      l += (j - l - 1 >= 0).int
      while j + r + 1< n and lines[i][j + r + 1] < me: inc r
      r += (j + r + 1< n).int
      while i - t - 1>= 0 and lines[i - t - 1][j] < me: inc t
      t += (i - t - 1>= 0).int
      while i + d + 1 < n and lines[i + d + 1][j] < me: inc d
      d += (i + d + 1 < n).int
      gold = max(gold, l * d * t * r)
  echo gold

when isMainModule:
  main()
