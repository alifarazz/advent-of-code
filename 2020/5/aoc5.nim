import os, sugar, sequtils, algorithm

const columnCount = 3

func binSearch(str: string, leftChar: char): int =
  var
    left = 0
    right = (1 shl str.len) - 1
  for c in str:
    let mid = (left + right) shr 1
    if c == leftChar:
      right = mid
    else:
      left = mid + 1
  left

when isMainModule:
  let path = if paramCount() < 1: "input" else: paramStr(1)
  let ids = toSeq(path.lines).map(line =>
    binSearch(line[0 .. ^(columnCount + 1)], 'F') shl columnCount +
    binSearch(line[^(columnCount) .. ^1], 'L')).sorted()
  echo ids[^1]
  for i in 0 .. ids.len - 2:
    if ids[i] + 1 != ids[i + 1]:
      echo ids[i] + 1
      break
