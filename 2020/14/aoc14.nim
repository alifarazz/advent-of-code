import strutils, sequtils, math, tables, strscans

func processMask(smask: string, andMask, orMask: var int): void =
  andMask = -1
  orMask = 0
  for i, b in smask:
    case b:
      of '1': orMask = orMask or (1 shl (35 - i))
      of '0': andMask = andMask and not(1 shl (35 - i))
      else: discard

func findOffsets(smask: string): seq[int] =
  var indices: seq[int]
  for i, ch in smask:
    if ch == 'X':
      indices.add(35 - i)
  result.setLen(1 shl indices.len)
  for i in 0 ..< (1 shl indices.len):
    var offset: int
    for j, digit in indices:
      if (i and (1 shl j)).bool:
        offset = offset or 1 shl digit
    result[i] = offset

func findFirstAddress(address, zeromask: int, smask: string): int =
  result = address or zeromask
  for i, c in smask:
    if c == 'X':
      result = result and not(1 shl (35 - i))

when isMainModule:
  static: assert 4 < sizeof(int), ": int type must be more than 32-bits"

  var andMask, orMask, zeroMask: int
  var smask: string
  var offsets: seq[int]
  var mem1, mem2: Table[int, int]

  for line in lines(stdin):
    if line.scanf("mask = $+", smask):
      processMask(smask, andMask, orMask) #  part1
      offsets = findOffsets(smask)
      zeroMask = smask.replace("X", "0").parseBinInt
    else:
      var address, value: int
      doAssert line.scanf("mem[$i] = $i", address, value)
      mem1[address] = value and andMask or orMask
      let firstAddr = address.findFirstAddress(zeroMask, smask)
      for offset in offsets:
        mem2[firstAddr or offset] = value

  echo toSeq(mem1.values).sum
  echo toSeq(mem2.values).sum
