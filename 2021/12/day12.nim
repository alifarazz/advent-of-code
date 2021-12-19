import std/[strutils, sets, tables]

var atable: Table[string, seq[string]]
var seen: HashSet[string]

proc insertInto[A, B](t: var Table[A, seq[B]]; key: A; val: sink B) =
  if not t.hasKey(key):
    t[key] = @[]
  t[key].add(val)

proc dfs1(me: string): int =
  for child in atable[me]:
    if child == "end":
      result.inc
    elif child[0].isUpperAscii or child notin seen:
      let isLower = child[0].isLowerAscii
      if isLower: seen.incl child
      result += dfs1(child)
      if isLower: seen.excl child

var seenTwice: bool
proc dfs2(me: string): int =
  for child in atable[me]:
    let isLower = child[0].isLowerAscii
    if child == "end":
      result.inc
    elif isLower.not or child notin seen:
      if isLower: seen.incl child
      result += dfs2(child)
      if isLower: seen.excl child
    elif isLower and seenTwice.not and child != "start" and child in seen:
      seenTwice = true
      result += dfs2(child)
      seenTwice = false

when isMainModule:
  for line in stdin.lines:
    let
      lr = line.split('-')
    atable.insertInto(lr[0], lr[1])
    atable.insertInto(lr[1], lr[0])

  seen = ["start"].toHashSet
  echo dfs1("start")
  seen = ["start"].toHashSet
  echo dfs2("start")
