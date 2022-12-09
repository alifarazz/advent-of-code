import std/[sets, strutils]

const knots_len = 10

type XY = (int, int)

func `-`(x, y: XY): XY = (x[0] - y[0], x[1] - y[1])
func `+`(x, y: XY): XY = (x[0] + y[0], x[1] + y[1])
func `+=`(x: var XY, y: XY) = x = x + y
func clamp(x: XY, a, b: int): XY = (x[0].clamp(a, b), x[1].clamp(a, b))

proc update_head(h: var XY, dir: char) =
  h[0] += (dir == 'R').int - (dir == 'L').int
  h[1] += (dir == 'U').int - (dir == 'D').int

proc update_tail(t: var XY, h: XY) =
  let diff = h - t
  if diff[0] * diff[0] + diff[1] * diff[1] > 2:
    t += diff.clamp(-1, 1)

proc main() =
  var head, tail: XY
  var knots: array[knots_len, XY]
  var silver_seen = toHashSet[XY]([(0, 0)])
  var gold_seen = toHashSet[XY]([(0, 0)])
  for line in stdin.lines():
    let dir: char = line[0]
    let nstep: int = line[2..^1].parseInt
    for i in 1..nstep:
      update_head(head, dir)
      update_tail(tail, head)
      silver_seen.incl(tail)
      update_head(knots[0], dir)
      for j in 1..<knots.len:
        update_tail(knots[j], knots[j - 1])
      gold_seen.incl(knots[^1])

  echo silver_seen.len
  echo gold_seen.len

when isMainModule:
  main()
