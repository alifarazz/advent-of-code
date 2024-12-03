import std/[strutils, bitops]

func sane_parseint(a: string,  ans: var int): bool =
  result = true
  try:
    ans = a.parseInt
  except ValueError:
    result = false

when isMainModule:
  let input = stdin.readAll
  var acc_silver, acc_gold: int
  var switch = true
  for i in countup(0, input.len - 4):
    if input[i..i+3] == "do()":
      switch = true
    if i < input.len - 6 and input[i..i+6] == "don't()":
      switch = false
    if input[i..i+3] == "mul(":
      var ok = 0
      var p1, p2: int
      for j in countup(i + 3, input.len - 1): # \n is last char
        if input[j] == ',':
          if j - (i + 4) <= 3 and input[i+4..<j].sane_parseint(p1):
            inc ok
            p2 = j + 1
          else:
            break
        elif input[j] == ')':
          if j - p2 <= 3 and input[p2..<j].sane_parseint(p2):
            inc ok
          break
      acc_silver.inc (-int(ok == 2)).bitand(p1 * p2)
      acc_gold.inc (-int(ok == 2 and switch)).bitand(p1 * p2)
  echo acc_silver
  echo acc_gold
