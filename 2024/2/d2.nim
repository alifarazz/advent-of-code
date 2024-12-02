import std/[strutils, sequtils, sugar]

func line_is_safe(nums: openArray[int]): bool =
    let diff = zip(nums[0..^2], nums[1..^1]).mapIt(it[1] - it[0])
    let sgn_ok = diff.all(x => x < 0) or diff.all(x => x > 0)
    let diff_ok = diff.mapIt(it.abs).all(x => (x >= 1 and x <= 3))
    sgn_ok and diff_ok

when isMainModule:
  var nsafe = 0
  var nsafe2 = 0
  for line in stdin.lines:
    let nums = line.splitWhitespace.mapIt(parseInt(it))
    nsafe += nums.line_is_safe.int
    for i in countup(0, nums.len - 1):
      var nums2 = nums
      nums2.delete(i)
      if nums2.line_is_safe:
        inc nsafe2
        break
  echo nsafe
  echo nsafe2
