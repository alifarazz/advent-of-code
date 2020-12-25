proc play(maxRound, sz: int, arrangement: string): seq[int] =
  var cur: int
  var next: seq[int] = newSeq[int](sz + 1)

  var prev: int
  for c in arrangement:
    cur = c.ord - '0'.ord
    next[prev] = cur
    prev = cur

  for i in arrangement.len() + 1..<sz: # fill out the rest
    next[cur] = i
    cur = i

  # complete loop
  next[cur] = next[0]
  cur = next[0]

  proc circularDec(x: var int): void = (if x == 1: x = sz - 1 else: dec x)

  for i in 0..<maxRound:
    var triplet: array[3, int]
    triplet[0] = next[cur]
    triplet[1] = next[triplet[0]]
    triplet[2] = next[triplet[1]]

    var dest = cur
    dest.circularDec
    while dest in triplet:
      dest.circularDec

    next[cur] = next[triplet[2]]
    let tmp = next[dest]
    next[dest] = triplet[0]
    next[triplet[2]] = tmp

    cur = next[cur]
  next

when isMainModule:
  let input: string = stdin.readLine

  let next1 = play(100, 10, input)
  var cur = next1[1]
  while cur != 1:
    stdout.write cur
    cur = next1[cur]
  stdout.write '\n'

  let next2 = play(10_000_000, 1_000_000 + 1, input)
  echo next2[1] * next2[next2[1]]
