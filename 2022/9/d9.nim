import std/[packedsets, strutils]

const knots_len = 10

type XY = distinct uint64

proc `[]`[I: Ordinal](x: XY, i: I): int32 =
  let ShiftAmnt = i * 32
  let Mask = (if i == 0: 0x0000_0000_FFFF_FFFFu else: 0xFFFF_FFFF_0000_0000u)
  let Chunk = x and Mask
  let Result = uint64(Chunk shr ShiftAmnt)
  result = (
    if Result > 0x7FFF_FFFFu:
      -int32((0x80_000_000u - (Result - 0x7fffffffu) + 1u))
    else:
      Result.int32
    )

proc `and`[T: Ordinal or XY](x: XY, y: T): XY = XY(uint64(x) and uint64(y))
proc `or`[T: Ordinal or XY](x: XY, y: T): XY = XY(uint64(x) or uint64(y))
proc `shl`[T: Ordinal](x: XY, i: T): XY = XY(uint64(x) shl i)
proc `shr`[T: Ordinal](x: XY, i: T): XY = XY(uint64(x) shr i)
proc toXY(x, y: int32): XY = XY(uint32(x)) or (XY(y) shl 32)
proc `-`(x, y: XY): XY = toXY(x[0] - y[0], x[1] - y[1])
proc `+`(x, y: XY): XY = toXY(x[0] + y[0], x[1] + y[1])
proc `==`(x, y: XY): bool = x.uint64 == y.uint64
proc `+=`(x: var XY, y: XY) = x = x + y

proc update_head(h: var XY, dir: char) =
  h = toXY(h[0] + (dir == 'R').int32 - (dir == 'L').int32,
           h[1] + (dir == 'U').int32 - (dir == 'D').int32)

proc update_tail(t: var XY, h: XY) =
  let d0 = h[0] - t[0]
  let d1 = h[1] - t[1]
  if d0 * d0 + d1 * d1 > 2:
    t += toXY(d0.clamp(-1, 1), d1.clamp(-1, 1))

proc main() =
  var head, tail: XY
  var knots: array[knots_len, XY]
  var silver_seen, gold_seen: PackedSet[uint64]
  for line in stdin.lines():
    let dir: char = line[0]
    let nstep = line[2..^1].parseInt
    for i in 1..nstep:
      update_head(head, dir)
      update_tail(tail, head)
      silver_seen.incl(tail.uint64)
      update_head(knots[0], dir)
      for j in 1..<knots.len:
        update_tail(knots[j], knots[j - 1])
      gold_seen.incl(knots[^1].uint64)

  echo silver_seen.len
  echo gold_seen.len

when isMainModule:
  main()
