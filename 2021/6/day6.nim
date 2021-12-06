from std/sequtils import mapIt, foldl

var fishAmount: array[9, int]

proc sim(daySlice: Slice[int]): int =
  for day in daySlice:
    fishAmount[(day + 7) mod 9] += fishAmount[day mod 9]
  result = fishAmount.foldl(a + b)

when isMainModule:
  let input = stdin.readAll
  let state = (0..<(input.len div 2)).mapIt(input[2 * it].ord - '0'.ord)
  for dayCount in state:
    inc fishAmount[dayCount]
  echo sim 0..<80
  echo sim 80..<256
