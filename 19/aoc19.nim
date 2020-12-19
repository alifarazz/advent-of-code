import strutils, sequtils, tables, re, math

var rules: Table[int, string]
var part2: bool = false

proc genRegex(rule: string): string =
  var regex: string
  for s in rule.split:
    if part2 and s == "8":
      regex &= genRegex("42") & "+"
    elif part2 and s == "11":
      const depth = 4 #  Anything more and the regex engine will crash :))))
      let r = toSeq(1..depth).zip(["42 ", "31 "].repeat(depth))
                .mapIt(it[1][0].repeat(it[0]) & it[1][1].repeat(it[0]))
                .join("| ").strip
      regex &= genRegex r
    else:
      case s[0]:
        of '"':
          return $s[1]
        of '|':
          regex &= "|"
        else:
          if s[0].isDigit:
            let idx = s.parseInt
            regex &= genRegex rules[idx]
  return "(" & regex & ")"

when isMainModule:
  let input = stdin.readAll.split "\n\n"

  proc splitRule(s: string): void =
    let r = s.split(": ")
    rules[r[0].parseInt] = r[1]
  input[0].split("\n").apply splitRule

  let strs = input[1].split("\n")

  let regex1 = ("^" & genRegex(rules[0]) & "$").re
  echo strs.mapIt(it.match regex1).mapIt(it.ord).sum

  part2 = true
  let regex2 = ("^" & genRegex(rules[0]) & "$").re
  echo strs.mapIt(it.match regex2).mapIt(it.ord).sum
