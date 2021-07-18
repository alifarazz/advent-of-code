import strutils, sequtils, tables

func solve(nums: openArray[int], max_iter, init_size: int): int =
  var mem = newTable[int, int](init_size)
  for (k, v) in zip(nums, toSeq(countUp(1, nums.len))):
    mem[k] = v
  var lastnum = nums[^1]
  for i in mem.len ..< max_iter:
    let newnum = i - mem.getOrDefault(lastnum, i)
    mem[lastnum] = i
    lastnum = newnum
  lastnum

when isMainModule:
  let nums = stdin.readLine.split(",").map(parseInt)
  echo nums.solve(2020, 385)
  echo nums.solve(3_0000_000, 3611295)
