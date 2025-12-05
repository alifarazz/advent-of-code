#!/usr/bin/env python

fp = open(0, "r")
ranges_str, eggs_str = fp.read().split('\n\n')

ranges = []
for line in ranges_str.split():
    a, b = map(int, line.split('-'))
    ranges.append((a, b))

silver = 0
for c in map(int, eggs_str.split()):
    for a, b in ranges:
        if a <= c <= b:
            silver += 1
            break
# could also do binary search for
# silver but I'm too lazy.
print(silver)

ranges.sort()
furthest = -1
gold = 0
for a, b in ranges:
    if furthest <= a:
        gold += b - a + (furthest != a)
        furthest = b
    elif a < furthest < b:
        gold += b - furthest
        furthest = b

print(gold)
