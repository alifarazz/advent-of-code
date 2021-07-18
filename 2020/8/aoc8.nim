import strutils, sequtils

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

when isMainModule:
  var insts = stdin.readAll()[0 .. ^2].split("\n").map(newInstruction)
  echo "done parsing"
  # echo runMachine(insts, seen)[0]
  var
    seen = newSeq[bool](insts.len)
    flipSeen = newSeq[bool](insts.len)
    acc = 0
    pc = 0
    corruptionFound = false
    corruptionAcc = -2

  while not seen[pc]:
    seen[pc] = true
    if not corruptionFound and insts[pc].flip():
      var
        flipPC = pc
        flipAcc = acc
      insts[flipPC].evaluate(flipAcc, flipPC) # advance once by hand to avoid
                                              # setting flipSeen[pc] to true
      while 0 <= flipPC and flipPC < insts.len and not (seen[flipPC] or flipSeen[flipPC]):
        flipSeen[flipPC] = true
        insts[flipPC].evaluate(flipAcc, flipPC)
      if flipPC == insts.len:
        corruptionFound = true
        corruptionAcc = flipAcc
      discard insts[pc].flip()
    insts[pc].evaluate(acc, pc)

  echo acc
  echo corruptionAcc
