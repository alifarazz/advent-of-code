# do a `$ nimble install fusion` before compiling
# compile with `$ nim c -d:danger -d:lto --passC:"-march=native" --mm:arc d7.nim` 

import std/[sequtils, strutils, sugar]
import fusion/matching
{.experimental: "caseStmtMacros".}

type FS = object
  cwd: seq[int]  # dirs in current path
  dirs: seq[int] # visisted dirs

proc initFS(): FS = FS(cwd: @[], dirs: @[])

func cd(fs: var FS, name: string) =
  if name == "..":
    fs.dirs.add(fs.cwd.pop())
  else:
    fs.cwd.add(0)

func addFile(fs: var FS, size: int) = fs.cwd.apply(s => s + size)

proc main() =
  var fs = initFS()

  for l in stdin.lines(): # crawl cwd
    case l.split():
      of ["$", "cd", @dir]: fs.cd(dir)
      of ["$", _]: continue
      of ["dir", _]: continue
      of [@size, _]: fs.addFile(size.parseInt)

  let sz = concat(fs.dirs, fs.cwd)

  const
    # total  = 3000000000
    # needed = 700000000
    total  = 70000000
    needed = 30000000
  echo sz.filter(sz => sz <= 100000).foldl(a + b)
  let max_sz = sz.max()
  echo sz.filter(sz => total - max_sz + sz >= needed).min()

when isMainModule:
  main()
