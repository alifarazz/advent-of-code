import sequtils, math, tables

type Coord = (int, int, int, int)

when isMainModule:
  var mem: Table[Coord, bool]
  var maxX, maxY: int

  for x, line in toSeq(stdin.lines):
    for y, c in line:
      if c == '#':
        mem[(x, y, 0, 0)] = true
        maxY = max(maxY, y)
    maxX = max(maxX, x)

  var maxZ, maxW: int
  var minX, minY, minZ, minW: int
  let dis = [-1, 0, 1]

  for cycle in 0..<6:
    var new_mem = mem
    dec minX
    dec minY
    dec minZ
    dec minW
    inc maxX
    inc maxY
    inc maxZ
    inc maxW
    for x in minX..maxX:
      for y in minY..maxY:
        for z in minZ..maxZ:
          for w in minW..maxW:
            var n = 0
            for dx in dis:
              for dy in dis:
                for dz in dis:
                  for dw in dis:
                    if dz == 0 and dx == 0 and dy == 0 and dw == 0:
                      continue
                    n += mem.getOrDefault((x+dx, y+dy, z+dz, w+dw), false).ord
            let cell = mem.getOrDefault((x, y, z, w), false)
            if not cell and n == 3:
              new_mem[(x, y, z, w)] = true
            elif not (cell and (n == 2 or n == 3)):
              new_mem[(x, y, z, w)] = false
    mem = new_mem
  echo toSeq(mem.values).mapIt(it.ord).sum
