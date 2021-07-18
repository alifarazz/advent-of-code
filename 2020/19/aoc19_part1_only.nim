import sequtils, strutils, pegs, math

var pegGrammer = "expr <- r0"

when isMainModule:
  let input = stdin.readAll.split("\n\n")
  # echo input[0]
  for line in input[0].split("\n"):
    let tks = line.split(": ")
    let ruleNumber = tks[0].parseInt
    case tks[1][0]:
      of '"':
        pegGrammer &= "\nr" & $ruleNumber & " <- \'" & $tks[1][1] & "\'"
      else:
        let rules = tks[1].split(" | ").mapIt(it.split)
        pegGrammer &= "\nr" & $ruleNumber & " <- "
        pegGrammer &= "("
        for rule in rules:
          pegGrammer &= "("
          for ruleNum in rule:
            pegGrammer &= "r" & $ruleNum & " "
          pegGrammer[^1] = ')'
          pegGrammer &= " "
          pegGrammer &= "/ "
        pegGrammer[^2] = ')'
  # echo pegGrammer
  let pegAst = pegGrammer.peg
  let strs = input[1][0..^2].split("\n")
  echo "Part one: "
  echo strs.mapIt(it.len == it.matchLen pegAst).mapIt(it.ord).sum
