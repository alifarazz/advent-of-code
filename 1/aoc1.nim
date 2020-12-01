import strutils
from sequtils import map

when isMainModule:
  var str = readFile("input").split()
  discard str.pop()
  let data = str.map(parseInt)

  block day1:
    for i in data:
      for j in data:
        if (i + j) == 2020:
          echo i * j
          break day1

  block day2:
    for i in data:
      for j in data:
        for k in data:
          if (i + j + k) == 2020:
            echo i * j * k
            break day2
