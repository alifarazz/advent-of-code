import strutils
import sequtils

type
  Password = object
    a: int
    b: int
    letter: char
    password: string

proc passwordIsValid1(pass: Password): bool =
  let letter_count = count(pass.password, pass.letter)
  pass.a <= letter_count and letter_count <= pass.b

proc passwordIsValid2(pass: Password): bool =
  pass.password[pass.a - 1] == pass.letter xor
  pass.password[pass.b - 1] == pass.letter

proc parseInputEntry(input: seq[string]): Password =
  let
    bounds = input[0].split('-').map(parseInt)
    letter = input[1][0]
    password = input[2]
  Password(a: bounds[0], b: bounds[1], letter: letter, password: password)

when isMainModule:
  var str = readFile("input").split()[0 .. ^2]
  let data = str.distribute(int(str.len / 3), false).map(parseInputEntry)
  echo count(map(data, passwordIsValid1), true)
  echo count(map(data, passwordIsValid2), true)
