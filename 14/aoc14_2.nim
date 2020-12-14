import strutils, sequtils, math, tables

proc findFirstAddress(address: int, smask: string): int =
  var firstAddress = address or smask.replace("X", "0").parseBinInt
  # echo firstAddress.toBin 10
  for i, c in smask:
    if c == 'X':
      firstAddress = firstAddress and not(1 shl (35 - i))
  firstAddress

when isMainModule:
  var smask: string
  var mem = initTable[int, int]()
  var idxs = newSeq[int]()

  for line in lines(stdin):
    let expr = line.split(" = ")
    if expr[0][1] == 'a':
      smask = expr[1]
      idxs.setLen(0)
      for i, c in smask:
        if c == 'X':
          idxs.add(35 - i)
      # echo idxs
    else:
      let address = expr[0][4..^2].parseInt
      let firstAddress = address.findFirstAddress(smask)
      let value = expr[1].parseInt
      for i in 0 ..< (1 shl idxs.len):
        var offset: int
        for j, digit in idxs:
          if (i and (1 shl j)).bool:
            offset = offset or 1 shl digit
        let address = offset or firstAddress
        mem[address] = value
        # echo "\t", address
      # echo line
      # quit()
  echo toSeq(mem.values).sum
