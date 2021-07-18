import strutils, sequtils, re, sets, math
import neo

type Rule = object
  bounds: array[4, int]
  name: string

func checksOut(ticketItem: int, r: Rule): bool =
  ticketItem in r.bounds[0] .. r.bounds[1] or
    ticketItem in r.bounds[2] .. r.bounds[3]

proc ticketIsLegal(items: openArray[int], rules: openArray[Rule]): int =
  var totalWrongs = items.toHashSet
  for rule in rules:
    let wrong = items.filterIt(it.checksOut(rule)).toHashSet
    totalWrongs = totalWrongs - wrong
    if totalWrongs.len == 0:
      return 0
  return toSeq(totalWrongs).sum

when isMainModule:
  let input = stdin.readAll[0..^2].split("\n\n")
  var rules: seq[Rule]

  for l in input[0].split("\n"):
    var matches: array[5, string]
    doAssert l.match(re"([a-z\s]*):\s([0-9]+)-([0-9]+)\sor\s([0-9]+)-([0-9]+)",
        matches = matches)
    let bounds = matches[1 .. ^1].map(parseInt)[0..3]
    let s = matches[0] # to prevent the implicit copy of matches[0] due
                       # to sink being buggy
    rules.add Rule(bounds: [bounds[0], bounds[1], bounds[2], bounds[3]],
        name: s)

  let tickets = input[2].split("\n")[1 .. ^1].mapIt(it.split(",").map(parseInt))
  let invalidTickets = tickets.mapIt(it.ticketIsLegal rules)
  echo invalidTickets.sum # part 1

  let validIdxs = vector(invalidTickets.zip(toSeq(
      0..<invalidTickets.len)).filterIt(it[0] == 0).unzip()[1])
  let validTickets = matrix(@[input[1].split("\n")[1].split(",").map(
      parseInt)] & validIdxs.mapIt(tickets[it]))

  var possibleMatchings = newSeqWith(validTickets.N, initHashSet[int]())
  for i, rule in rules: # find possible column->rule matchings
    var j = 0
    for column in validTickets.columns:
      if column.mapIt(it.checksOut(rule)).allIt(it == true):
        possibleMatchings[j].incl i
      inc j

  var product = 1
  for matchedCount in 0 ..< possibleMatchings.len: # find the matching
    let idx = possibleMatchings.mapIt(it.len == 1).find(true)
    let ruleIdx = possibleMatchings[idx].pop
    if "departure" in rules[ruleIdx].name:
      product *= validTickets[0, idx]
    for i in 0 ..< possibleMatchings.len:
      possibleMatchings[i].excl ruleIdx
  echo product # part 2
