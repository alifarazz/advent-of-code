import strutils
import sugar
import sequtils

const slopes = [(3, 1), (1, 1), (5, 1), (7, 1), (1, 2)]
type slopeType = (int, int)

func traverse_slope(str: openArray[string], slope: slopeType): int =
  let line_len = str[0].len
  var
    i = 0
    j = 0
  while i < str.len - 1:
    i += slope[1]
    j = (j + slope[0]) mod line_len
    result += int(str[i][j] == '#')

when isMainModule:
  let str = readFile("input").split()[0 .. ^2]

  let part1 = traverse_slope(str, slopes[0])
  let part2 = part1 * slopes[1 .. ^1].map(x => traverse_slope(str, x)).foldl(a * b)

  echo part1
  echo part2
