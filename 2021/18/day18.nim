import std/[sequtils, strutils, strformat]

proc getSplitIdx(s: string): int =
  assert s.len > 1
  var stack: int
  for i, c in s[1..^1]:
    case c:
      of '[':
       inc stack
      of ']':
        dec stack
      of ',':
        if stack == 0:
           return i
      else:
        discard

type
  Node = ref object
    le, ri: Node
    parent {.cursor.}: Node
    data: int

proc makeNode(s: string): Node =
  new result
  if s.len == 1:
    result.data = s.parseInt
  else:
    var
      spIdx = s.getSplitIdx
      le = s[1..spIdx]
      ri = s[(spIdx+2)..^2]
    result.le = makeNode le
    result.ri = makeNode ri
    result.le.parent = result
    result.ri.parent = result
    result.data = -1

proc `$`(n: Node): string =
  if n.data != -1:
     $n.data
  else:
    &"[{$n.le},{$n.ri}]"

proc `+`(a, b: Node): Node =
  new result
  result.data = -1
  result.le = a
  result.ri = b
  a.parent = result
  b.parent = result

proc split(me: Node): bool =
  if me.data >= 10:
    new me.le
    new me.ri
    me.le.parent = me
    me.ri.parent = me
    me.le.data = me.data div 2
    me.ri.data = me.data - me.le.data
    me.data = -1
    result = true
  elif me.le != nil and me.ri != nil:
    result = me.le.split() or me.ri.split()

proc nextInorder(me: Node): Node =
  var me = me
  while me.parent != nil:
    var parent = me.parent
    if parent.le == me:
      me = parent.ri
      while me.data == -1:
        me = me.le
      return me
    elif parent.ri == me:
      me = parent
  nil

proc prevInorder(me: Node): Node =
  var me = me
  while me.parent != nil:
    var parent = me.parent
    if parent.ri == me:
      me = parent.le
      while me.data == -1:
        me = me.ri
      return me
    elif parent.le == me:
      me = parent
  nil

proc explode(me: Node, depth: int = 0): bool =
  if me.le != nil and me.ri != nil and me.le.data != -1 and me.ri.data != -1:
    if depth >= 4:
      let next = me.nextInorder
      if next != nil:
        next.data += me.ri.data
      let prev = me.prevInorder
      if prev != nil:
        prev.data += me.le.data
      me.le = nil
      me.ri = nil
      me.data = 0
      result = true
  elif me.le != nil and me.ri != nil:
    result = me.le.explode(depth + 1) or me.ri.explode(depth + 1)

proc reduce(a, b: Node): Node =
  result = a + b
  while result.explode or result.split:
    discard

proc magnitude(me: Node): int =
  if me.data != -1:
    me.data
  else:
    let
      l = me.le.magnitude()
      r = me.ri.magnitude()
    3 * l + 2 * r

proc findMaxMagnitude(roots: openArray[string]): int =
  for i, a in roots:
    for j, b in roots:
      if i != j:
        let
          lhead = a.makeNode
          rhead = b.makeNode
        result = max(result, reduce(lhead, rhead).magnitude)

when isMainModule:
  let v = stdin.readAll[0..^2].splitLines
  let roots = v.map(makeNode)
  echo roots.foldl(reduce(a, b)).magnitude, '\n',
       v.findMaxMagnitude

#[
proc sanitized(me: Node): bool =
 if me.data != -1:
    result = me.data >= 0 and me.data < 10 and me.le == nil and me.ri == nil
 else:
   if me.le == nil or me.ri == nil:
     return false
   result = me.le.parent == me and me.ri.parent == me and me.le.sanitized and me.ri.sanitized
]#
