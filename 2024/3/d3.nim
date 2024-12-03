import std/bitops

func sane_parseint(a: string,  ans: var int): bool =
  result = true
  var local_ans = 0
  for i in countup(0, a.len - 1):
    case a[i]:
      of '0'..'9':
        local_ans *= 10
        local_ans += a[i].ord - '0'.ord
      else:
        result = false
        break
  ans = local_ans

when isMainModule:
  let input = stdin.readAll
  var acc_silver, acc_gold: int
  var switch = true
  var i = 0
  while i <= input.len - 4:
    if input[i..i+3] == "do()":
      switch = true
      i += 4
      continue
    if i < input.len - 6 and input[i..i+6] == "don't()":
      switch = false
      i += 7
      continue
    if input[i..i+3] == "mul(":
      var ok = 0
      var p1, p2: int
      var last_j = i + 3
      for j in countup(i + 3, input.len - 1): # \n is last char
        case input[j]
        of ',':
          if j - (i + 4) <= 3 and input[i+4..<j].sane_parseint(p1):
            inc ok
            last_j = j + 1
          else:
            break
        of ')':
          if j - last_j <= 3 and input[last_j..<j].sane_parseint(p2):
            inc ok
            last_j = j + 1
          break
        else:
          discard
      acc_silver.inc (-int(ok == 2)).bitand(p1 * p2)
      acc_gold.inc (-int(ok == 2 and switch)).bitand(p1 * p2)
      i = last_j
      continue
    inc i
  echo acc_silver
  echo acc_gold
