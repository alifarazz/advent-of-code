import strutils, sequtils, strformat, algorithm

when isMainModule:
  let jolts_in = stdin.readAll()[0 .. ^2].split("\n").map(parseInt).sorted()
  let jolts = @[0] & jolts_in & @[jolts_in[^1] + 3]

  var
    diff1 = 0
    diff3 = 0
    prev_jolt = 0
  for jolt in jolts:
    case jolt - prev_jolt:
      of 1: inc diff1
      of 3: inc diff3
      else: discard
    prev_jolt = jolt
  echo &"Part1:\n\t1s:\t{diff1}\n\t3s:\t{diff3}\n\tprod:\t{diff1 * diff3}"

  var dp = newSeq[uint64](4)
  dp[0] = 1
  for i in 1 ..< jolts.len:
    let idx = i mod 4
    dp[idx] = dp[(i - 1) mod 4]
    if i >= 2 and jolts[i] - jolts[i - 2] <= 3:
      dp[idx] += dp[(i - 2) mod 4]
    if i >= 3 and jolts[i] - jolts[i - 3] <= 3:
      dp[idx] += dp[(i - 3) mod 4]
  echo &"Part2:\n\t{dp[(jolts.len - 1) mod 4]}"
