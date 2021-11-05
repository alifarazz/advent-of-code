import strutils, intsets
from sequtils import map

when isMainModule:
  var str = readFile("input").split()
  discard str.pop()
  let data = str.map(parseInt)
  let dataSet = data.toIntSet()

  block day1:
    for i in data:
      let j = 2020 - i
      if j in dataSet:
        echo i * j
        break day1

  block day2:
    for i in data:
      for j in data:
        let k = 2020 - i - j
        if k in dataSet:
          echo i * j * k
          break day2
