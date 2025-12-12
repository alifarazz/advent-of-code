#!/usr/bin/env python

input = open(0).read().split("\n\n")
gifts = [s.count("#") for s in input[:-1]]

def solve(line):
    wh, counts = line.split(": ")
    w, h = map(int, wh.split("x"))
    counts = map(int, counts.split())
    target = sum(map(lambda x: x[0] * x[1], zip(counts, gifts)))
    return (w * h) > target 

print(sum(map(solve, input[-1].split("\n")[:-1])))
