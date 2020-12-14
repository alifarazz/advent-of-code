import strutils, sequtils, math, tables

proc processMask(smask: string, andMask, orMask: var int): void =
  andMask = -1
  orMask = 0
  for i, b in smask:
    case b:
      of '1':
        orMask = orMask or (1 shl (35 - i))
      of '0':
        andMask = andMask and not(1 shl (35 - i))
      else:
        discard
  # echo smask
  # echo andMask.toBin 36
  # echo orMask.toBin 36
  # quit()

when isMainModule:
  var andMask, orMask:int
  var mem = initTable[int, int]()

  for line in lines(stdin):
    let expr = line.split(" = ")
    if expr[0][1] == 'a':
      processMask(expr[1], andMask, orMask)
    else:
      let address = expr[0][4..^2].parseInt
      let value = expr[1].parseInt and andMask or orMask
      # echo "address:", "\t", value and andMask or orMask
      mem[address] = value
  echo toSeq(mem.values).sum
