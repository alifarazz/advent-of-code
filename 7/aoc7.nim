import sugar, sequtils, strutils, tables, sets, math

var seen = initHashSet[string]()

func findGraph(bag: seq[string], graph: var TableRef[string, seq[(string, int)]],
               graphInverted: var TableRef[string, seq[string]]): void =
  let rootBag = bag[0]
  if bag[1][0] != 'n': # contain no other bags.
    let subBags = bag[1][0 .. ^2].split(", ")
    for b in subBags:
      let tokens = b.split()
      let name = tokens[1] & " " & tokens[2]
      if graphInverted.hasKeyOrPut(name, @[rootBag]):
        graphInverted[name].add(rootBag)
      let value = (name, tokens[0].parseInt())
      if graph.hasKeyOrPut(rootBag, @[value]):
        graph[rootBag].add(value)

proc dfs1(u: string, graph: TableRef[string, seq[string]]): int =
  if not seen.containsOrIncl(u):
    result = graph.getOrDefault(u).map(v => dfs1(v, graph)).sum() + 1

func dfs2(u: (string, int), graph: TableRef[string, seq[(string, int)]]): int =
  graph.getOrDefault(u[0]).map(v => dfs2(v, graph) * v[1]).sum() + 1

when isMainModule:
  let ls = stdin.readAll()[0 .. ^2].split("\n").map(x => x.split(" bags contain "))
  var
    graph = newTable[string, seq[(string, int)]]()
    graphInverted = newTable[string, seq[string]]()

  for bag in ls:
    findGraph(bag, graph, graphInverted)

  echo dfs1("shiny gold", graphInverted) - 1
  echo dfs2(("shiny gold", 1), graph) - 1
