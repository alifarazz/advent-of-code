import strutils, sequtils, sets

func tokenize(s: string): seq[string] =
  var i: int
  while i < s.len:
    if s[i] == 'n' or s[i] == 's':
      result.add s[i..i+1]
      inc i, 2
    else:
      result.add s[i..i]
      inc i

func whites(black: (int, int)): seq[(int, int)] =
  for m in ["nw", "sw", "w", "ne", "se", "e"]:
    var (x, y) = black
    case m:
      of "nw":
        x -= 1
        y += 1
      of "sw":
        x -= 1
        y -= 1
      of "ne":
        x += 1
        y += 1
      of "se":
        y -= 1
        x += 1
      of "w": x -= 2
      of "e": x += 2
    result.add (x, y)

func countBlacks(white:(int, int), blacks: HashSet[(int, int)]): int =
  for m in ["nw", "sw", "w", "ne", "se", "e"]:
    var (x, y) = white
    case m:
      of "nw":
        x -= 1
        y += 1
      of "sw":
        x -= 1
        y -= 1
      of "ne":
        x += 1
        y += 1
      of "se":
        y -= 1
        x += 1
      of "w": x -= 2
      of "e": x += 2
    result += ((x, y) in blacks).ord


when isMainModule:
  var blacks: HashSet[(int, int)]
  for dirs in stdin.readAll[0..^2].splitLines.map(tokenize):
    var x, y: int
    for m in dirs:
      case m:
        of "nw":
          x -= 1
          y += 1
        of "sw":
          x -= 1
          y -= 1
        of "ne":
          x += 1
          y += 1
        of "se":
          y -= 1
          x += 1
        of "w": x -= 2
        of "e": x += 2
    if (x, y) notin blacks:
      blacks.incl (x, y)
    else:
      blacks.excl (x, y)
  echo blacks.len

  for i in 0..<100:
    var newBlacks = blacks
    for black in blacks:
      var n: int = black.countBlacks(blacks)
      if n == 0 or n > 2:
        newBlacks.excl black
      for white in black.whites:
        if white.countBlacks(blacks) == 2:
          newBlacks.incl white
    blacks = newBlacks
  echo blacks.len
