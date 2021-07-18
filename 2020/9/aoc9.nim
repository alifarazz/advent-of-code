import strutils, sequtils, math #, strformat

when isMainModule:
  let input = stdin.readAll()[0 .. ^2].split("\n").map(parseInt)
  let inputScan = input.cumsummed()
  let preambleLen = 25

  var badnum = -1
  for i in preambleLen ..< input.len:
    let target = input[i]
    var found = false
    for j in i - preambleLen .. i:
      let a = input[j]
      if not (target - a != a and target - a notin input[i - preambleLen ..< j]):
        found = true
        break
    if not found:
      badnum = target
      echo badnum
      break

  block gold:
    for i in 1 ..< inputScan.len:
      for j in 0 ..< i:
        if inputScan[i] - inputScan[j] == badnum:
          echo input[j + 1 .. i].min() + input[j + 1 .. i].max()
          # echo fmt"{input[i]}, {input[j+1]}"
          break gold
