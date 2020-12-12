import strutils, sequtils

type Action = object
  units: int
  dir: char

func newAction(s: string): Action =
  Action(dir: s[0], units: s[1..^1].parseInt())

const
  sin = [0, 1, 0, -1]
  cos = [1, 0, -1, 0]

func rotate(theta: int, wpx, wpy: var int): void =
  (wpx, wpy) = (wpx * cos[theta] - wpy * sin[theta],
                wpx * sin[theta] + wpy * cos[theta])

when isMainModule:
  let actions = stdin.readAll()[0..^2].splitLines().map(newAction)

  block part1:
    var x, y, d: int
    for a in actions:
      case a.dir:
        of 'N': y += a.units
        of 'S': y -= a.units
        of 'W': x -= a.units
        of 'E': x += a.units
        of 'L': d = (d + a.units div 90) %% 4
        of 'R': d = (d - a.units div 90) %% 4
        of 'F':
          x -= a.units * sin[d]
          y -= a.units * cos[d]
        else: quit(-1)
    echo abs(x) + abs(y)

  block part2:
    var x, y: int
    var
      wpx = -10
      wpy = 1
    for a in actions:
      case a.dir:
        of 'N': wpy += a.units
        of 'S': wpy -= a.units
        of 'W': wpx += a.units
        of 'E': wpx -= a.units
        of 'L': rotate(-a.units div 90 %% 4, wpx, wpy)
        of 'R': rotate( a.units div 90 %% 4, wpx, wpy)
        of 'F':
          x -= a.units * wpx
          y -= a.units * wpy
        else: quit(-2)
    echo abs(x) + abs(y)
