import sequtils

proc toID(str: string): uint64 =
  for c in str:
    result = (result shl 1) or uint64(c == 'R' or c == 'B')

when isMainModule:
  let ids = toSeq(stdin.lines()).map(toID)
  let max = uint64 ids.max()
  let min = uint64 ids.min() - 1
  echo max
  echo (max * (max + 1) - min * (min + 1)) shr 1 - ids.foldl(a + b)
