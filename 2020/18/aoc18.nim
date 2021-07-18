import sequtils, strutils, math

func evalExpr(expr: string, part1: bool): int =
  var operatorStack: seq[char]
  var operandStack: seq[int]

  proc evalTerm(): void =
    let a = operandStack.pop
    let b = operandStack.pop
    let op = operatorStack.pop
    case op:
      of '+': operandStack.add (a + b)
      of '*': operandStack.add (a * b)
      else: discard

  for token in expr:
    case token:
      of '(': operatorStack.add token
      of ')':
        while operatorStack[^1] != '('
          # and operatorStack.len > 0
          :
          evalTerm()
        if operatorStack[^1] == '(':
          discard operatorStack.pop
      of '*', '+':
        while operatorStack.len > 0 and
              operatorStack[^1] != '(' and
              (part1 or not(part1) and
                        token == '*' and
                        operatorStack[^1] == '+'):
          evalTerm()
        operatorStack.add token
      else: # then it's has to be a single digit number
        operandStack.add (token.ord - '0'.ord)

  while operatorStack.len > 0:
    evalTerm()
  operandStack.pop

when isMainModule:
  let exprs: seq[string] = stdin.readAll[0..^2].split("\n").mapIt it.replace(" ")
  echo exprs.mapIt(it.evalExpr(true)).sum
  echo exprs.mapIt(it.evalExpr(false)).sum
