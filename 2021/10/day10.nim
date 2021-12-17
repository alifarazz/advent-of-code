import std/[sequtils, strutils, algorithm,]

const
  closing = [')', ']', '}', '>']
  opening = ['(', '[', '{', '<']
  scores  = [3, 57, 1197, 25137]

proc processLine(line: string): (int, int) =
  var stack: seq[char]
  for c in line:
    if c in opening:
      stack.add c
    else:
      let
        opIdx = opening.find(stack.pop)
        clIdx = closing.find(c)
      if opIdx != clIdx:
        result[0] = scores[clIdx]
  if result[0] == 0:
    for i in stack.reversed().mapIt(opening.find(it)):
      result[1] = result[1] * 5 + i + 1

when isMainModule:
  let v = stdin.readAll[0..^2].splitLines
  let scs = v.map(processLine).unzip
  let scs2 = scs[1].filterIt(it != 0).sorted()
  echo scs[0].foldl(a + b), '\n',
       scs2[scs2.len div 2]

  # for i, line in v:
  #   echo i
  #   let x = line.processLine
