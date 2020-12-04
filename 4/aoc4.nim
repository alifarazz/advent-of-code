import re, os, sugar, strutils, sequtils

const requiredFields = ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"]
const eyeColors = ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"]

func passportIsValid2(passport: seq[seq[string]]): bool =
  func checkInt(value: string, low, high: int): bool =
    let v = parseInt(value)
    low <= v and v <= high

  func fieldIsValid(key_value: seq[string]): bool =
    let (key, value) = (key_value[0], key_value[1])
    case key:
      of "byr":
        value.len == 4 and checkInt(value, 1920, 2002)
      of "iyr":
        value.len == 4 and checkInt(value, 2010, 2020)
      of "eyr":
        value.len == 4 and checkInt(value, 2020, 2030)
      of "hgt":
        if value[^2 .. ^1] == "cm":
          checkInt(value[0 .. ^3], 150, 193)
        else:
          checkInt(value[0 .. ^3], 59, 76)
      of "hcl":
        match(value, re"#([0-9a-f]{6})")
      of "ecl":
        eyeColors.map(x => x == value).anyIt(it)
      of "pid":
        discard parseInt(value)
        value.len == 9
      else:
        true
  try:
    passport.map(fieldIsValid).allIt(it)
  except:
    false

func passportIsValid1(passport: seq[string]): bool =
  requiredFields.map(x => x in passport).allIt(it)

when isMainModule:
  let path = if paramCount() < 1: "input" else: paramStr(1)
  let input = readFile(path)[0 .. ^2].split("\n\n").map(x => x.split()).map(
      x => x.map(y => y.split(':')))
  let
    pt1 = input.map(x => passportIsValid1(x.map(y => y[0])))
    pt2 = input.map(passportIsValid2)
  echo pt1.countIt(it)
  echo zip(pt1, pt2).countIt(it[0] and it[1])
