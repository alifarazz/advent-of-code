import strutils, sequtils, algorithm

type
  InstructionType {.pure.} = enum
    JMP, NOP, ACC

type
  Instruction = object
    itype: InstructionType
    operand: int

func flip(ins: var Instruction): bool =
  if ins.itype == JMP or ins.itype == NOP:
    ins.itype = [JMP, NOP][int(ins.itype == JMP)]
    result = true

func newInstruction(s: string): Instruction =
  Instruction(operand: s[4 .. ^1].parseInt(),
              itype: case s[0]:
    of 'j': JMP
    of 'n': NOP
    else: ACC)

func evaluate(ins: Instruction, acc, pc: var int): void =
  case ins.itype:
    of JMP:
      pc += ins.operand
    of NOP:
      inc pc
    of ACC:
      acc += ins.operand
      inc pc

func runMachine(insts: openArray[Instruction], seen: var openArray[bool]): (int, bool) =
  var
    acc = 0
    pc = 0
  while pc < seen.len and not seen[pc]:
    seen[pc] = true
    insts[pc].evaluate(acc, pc)
  (acc, pc != insts.len) # (acc, did you loop?)

when isMainModule:
  var insts = stdin.readAll()[0 .. ^2].split("\n").map(newInstruction)
  var
    seen = newSeq[bool](insts.len)
    doesTerminate = newSeq[bool](insts.len)
  doesTerminate.fill(true)
  echo "done parsing"
  echo runMachine(insts, seen)[0]

  for i, ins in insts:
    if insts[i].flip():
      let (acc, didLoop) = runMachine(insts, seen)
      if not didLoop:
        echo "\n", acc
        break
      discard insts[i].flip()
      seen.fill(false)
