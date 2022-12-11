import std/[strutils, sequtils]

type Monkey = object
  Items: seq[uint64]
  OpIsMul: bool
  OperandR: int
  Div: int
  True: int
  False: int
  Business: int

func `$`(m: Monkey): string = "Monkey(" & $m.Items & ", " & $m.OpIsMul & ", " &
    $m.OperandR & ", " & $m.Div & ", " & $m.True & ", " & $m.False & ")"

func makeMonkey(sItems, sOps, sTest, sTrue, sFalse: string): Monkey =
  result.Items = sItems[18..^1].split(", ").mapIt(it.parseInt().uint64)
  result.OpIsMul = sOps[23] == '*'
  let sOpR = sOps[25..^1]
  result.OperandR = (if sOpR == "old": -1 else: sOpR.parseInt)
  result.Div = sTest[21..^1].parseInt
  result.True = sTrue[29..^1].parseInt
  result.False = sFalse[30..^1].parseInt

func FindTopTwo(monkeys: openArray[Monkey]): (int, int) =
  var first, second = 0
  for i in 0..<monkeys.len:
    let b = monkeys[i].Business
    if b > first:
      second = first
      first = b
    elif b > second:
      second = b
  (first, second)

proc ParseInputMonkey(ms: var seq[Monkey]) =
  let _ = stdin.readLine
  let sItems = stdin.readLine
  let sOps = stdin.readLine
  let sTest = stdin.readLine
  let sTrue = stdin.readLine
  let sFalse = stdin.readLine
  ms.add(makeMonkey(sItems, sOps, sTest, sTrue, sFalse))

func Tick(monkeys: var openArray[Monkey], f: proc (x: uint64): uint64 {.noSideEffect.}) =
  for m in monkeys.mitems:
    m.Business += m.Items.len
    for item in m.Items:
      let r = (if m.OperandR < 0: item else: m.OperandR.uint64)
      let x = (if m.OpIsMul: item * r else: item + r)
      let newItem = f(x)
      let dest = (if newItem mod m.Div.uint64 == 0: m.True else: m.False)
      monkeys[dest].Items.add(newItem)
    m.Items.setLen(0)

func Sim(m: var openArray[Monkey], f: proc (x: uint64): uint64 {.noSideEffect.}, n: uint64): int =
  for _ in 1..n: Tick(m, f)
  let (a, b) = m.FindTopTwo()
  a * b

proc main() =
  var m1, m2: seq[Monkey]

  ParseInputMonkey(m1)
  while not stdin.endOfFile():
    let _ = stdin.readChar
    ParseInputMonkey(m1)
  m2 = m1

  block silver:
    func f(x: uint64): uint64 = x div 3
    echo Sim(m1, f, 20)
  block gold:
    let divs = m2.mapIt(it.Div.uint64).foldl(a * b)
    func f(x: uint64): uint64 = x mod divs
    echo Sim(m2, f, 10_000)

when isMainModule:
  main()
