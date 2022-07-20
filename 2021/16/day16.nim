const WeAreTesting = false

import std/sequtils
when WeAreTesting:
  import std/[strutils, strformat]

const # NOTE: Data dependent!!!! know your data
  MAXN = 1 shl 13
  MAXSTK = 1 shl 6

type MyStaticStack = object
  arr: array[MAXSTK, uint]
  sz: int

var 
  pktBool: array[MAXN, bool]
  stk: MyStaticStack

func add(this: var MyStaticStack, e: uint) =
  this.arr[this.sz] = e
  inc this.sz
  
func pop(this: var MyStaticStack): uint =
  dec this.sz
  this.arr[this.sz]

proc asciiHexStringToBinSeq(s: string): int =
  for sc in s:
    let c = sc.ord
    let h = if c <= '9'.ord:
              c - '0'.ord
            else:
              c - 'A'.ord + 10
    pkt_bool[result + 0] = (h and 0b1000).bool
    pkt_bool[result + 1] = (h and 0b0100).bool
    pkt_bool[result + 2] = (h and 0b0010).bool
    pkt_bool[result + 3] = (h and 0b0001).bool
    result += 4
    
proc asciiBinStringToBinSeq(s: string): int =
  for sc in s:
    pkt_bool[result] = (sc.ord - '0'.ord).bool
  inc result

func parseBinInt(s: openArray[bool], offset, sz: int): int =
  var i = 0
  while i < sz:
    result = (result shl 1) or s[offset + i].int
    inc i

func unpackLiteralPacketData(s: openArray[bool], i: var int): uint =
  func once(s: openArray[bool], acc: var uint, i: var int) =
    let new = s.parseBinInt(i + 1, 4).uint
    acc = (acc shl 4) or new
    i += 5

  while s[i] == true:
    once(s, result, i)
  once(s, result, i)

proc parsePacketImpl(s: openArray[bool], offset: int = 0): (uint, int)

proc extractSubPacketData(s: openArray[bool], i: var int): int =
  let lenId = s[i]
  inc i
  if lenId == false:
    let subPacketLen = s.parseBinInt(i, 15)
    i += 15
    let oldI = i
    while i - oldI < subPacketLen:
      let subReturn = parsePacketImpl(s, i)
      stk.add(subReturn[0])
      inc result
      i += subReturn[1]
  else:
    var subPacketCount = s.parseBinInt(i, 11)
    i += 11
    while subPacketCount != 0:
      let subReturn = parsePacketImpl(s, i)
      stk.add(subReturn[0])
      inc result
      i += subReturn[1]
      dec subPacketCount

var sumVersion = 0
# TODO: use slices
proc parsePacketImpl(s: openArray[bool], offset: int): (uint, int) = # returns (data, i)
  var
    i = offset
    data: uint
  let
    v = s.parseBinInt(i, 3)
    t = s.parseBinInt(i + 3, 3)
  sumVersion += v
  i += 6
  case t:
    of 0:
      let n = extractSubPacketData(s, i)
      data = stk.arr[stk.sz-n..<stk.sz].foldl(a + b)
      stk.sz -= n
    of 1:
      let n = extractSubPacketData(s, i)
      data = stk.arr[stk.sz-n..<stk.sz].foldl(a * b)
      stk.sz -= n
    of 2:
      let n = extractSubPacketData(s, i)
      data = stk.arr[stk.sz-n..<stk.sz].foldl(min(a, b))
      stk.sz -= n
    of 3:
      let n = extractSubPacketData(s, i)
      data = stk.arr[stk.sz-n..<stk.sz].foldl(max(a, b))
      stk.sz -= n
    of 4:
      data = unpackLiteralPacketData(s, i)
    of 5:
      let n = extractSubPacketData(s, i)
      let r = stk.pop()
      let l = stk.pop()
      data = (l > r).uint
    of 6:
      let n = extractSubPacketData(s, i)
      let r = stk.pop()
      let l = stk.pop()
      data = (l < r).uint
    of 7:
      let n = extractSubPacketData(s, i)
      let r = stk.pop()
      let l = stk.pop()
      data = (l == r).uint
    else:
      quit(-1)
  (data, i - offset)

proc parsePacket(s: openArray[bool], offset: int = 0): (uint, int) = # returns (data, i)
  sumVersion = 0
  parsePacketImpl(s, offset)

when isMainModule and WeAreTesting:
  let binP1 = ["010100000010", "100100000100", "001100000110",
               "110100101111111000101000",
               "00111000000000000110111101000101001010010001001000000000",
               "11101110000000001101010000001100100000100011000001100000"]
  let hexP1 = ["8A004A801A8002F478", "620080001611562C8802118E34",
               "C0015000016115A2E0802F182340", "A0016C880162017C3686B18A3D4780"]
  let hexP2 = ["C200B40A82", "04005AC33890", "880086C3E88112", "CE00C43D881120",
               "D8005AC2A8F0", "F600BC2D8F", "9C005AC2F8F0",
               "9C0141080250320F1802104A08"]

  for i, g in hexP2:
    let packetLen = g.asciiHexStringToBinSeq()
    let p1 = pkt_bool.parsePacket()

    debugEcho(&"{\"-\".repeat(11)}CASE {i+1:0>2}{\"-\".repeat(11)}")
    debugEcho(&"Data:\t{p1[0]}")
    debugEcho(&"i:\t{p1[1]}")
    debugEcho(&"Vsum:\t{sumVersion}")


when isMainModule and WeAreTesting.not:
  let packetLen = stdin.readLine().asciiHexStringToBinSeq()
  let p = pkt_bool.parsePacket()
  echo sumVersion
  echo p[0]
