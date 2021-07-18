import strutils, sequtils, tables, math, sets, tables, algorithm

# from: https://github.com/nim-lang/Nim/pull/16333/
proc min*[T](x: openArray[T], cmp: proc(x, y: T): int {.closure.}): T =
  result = x[0]
  let compare =
    if cmp != nil:
      cmp
    else:
      system.cmp[T]

  for i in 1..high(x):
    if compare(x[i], result) < 0: result = x[i]

when isMainModule:
  var ingridCount: Table[string, int]
  var can: Table[string, HashSet[string]]

  var allIngrids: seq[string]
  for line in stdin.lines:
    let matches = line[0..^2].split " (contains "
    let ingridsSeq = matches[0].split
    allIngrids &= ingridsSeq
    let ingrids = ingridsSeq.toHashSet
    let allergens = matches[1].split ", "
    for i in ingrids:
      inc ingridCount.mgetOrPut(i, 0)
    # echo ingridCount
    for a in allergens:
      if can.hasKeyOrPut(a, ingrids):
        can[a] = can[a] * ingrids

  echo allIngrids.mapIt(it notin toSeq(can.values).sum).mapIt(it.ord).sum

  var ans: Table[string, string]
  while can.len > 0:
    var (k, v) = toSeq(can.pairs).min proc(x, y: (string, HashSet[
        string])): int = cmp(x[1].len, y[1].len)
    can.del k
    for i in can.mvalues:
      i = i - v
    ans[k] = v.pop

  echo toSeq(ans.pairs).sortedByIt(it[0]).unzip()[1].join ","
