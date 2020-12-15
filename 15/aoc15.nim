import strutils, sequtils, tables

func solve(nums: openArray[int], max_iter: int): int =
  var mem = zip(nums, toSeq(countUp(1, nums.len))).to_table
  var lastnum = nums[^1]
  for i in mem.len ..< max_iter:
    let newnum = i - mem.getOrDefault(lastnum, i)
    mem[lastnum] = i
    lastnum = newnum
  lastnum

when isMainModule:
  let nums = stdin.readLine.split(",").map(parseInt)
  echo nums.solve(2020)
  echo nums.solve(3_0000_000)
