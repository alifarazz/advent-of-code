import sugar, sequtils, strutils, bitops

func countQuestions(qs: openArray[string]): (int, int) =
  var
    seenPerPerson = 0u
    seenAll = 0xFFFF_FFFFu
  for q in qs:
    var seen = 0u
    for c in q:
      let oneHot = (1u shl (int(c) - int('a')))
      seen = seen or oneHot
      seenPerPerson = seenPerPerson or oneHot
    seenAll = seenAll and seen
  (popcount(seenPerPerson), popcount(seenAll))

when isMainModule:
  let input = stdin.readAll()[0 .. ^2].split("\n\n")
  let (part1, part2) = input.map(x => countQuestions(x.split())).unzip()
  echo part1.foldl(a + b), '\n', part2.foldl(a + b)
