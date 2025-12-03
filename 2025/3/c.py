#!/usr/bin/env python

import operator

fp = open(0, "r")
X = fp.read().split()

def find_largest(vs, n):
    acc = 0
    winl = 0  # window's left margin
    for i in list(range(-(n - 1), 0)) + [None]:
        idx, d = max(enumerate(vs[winl:i]), key=operator.itemgetter(1))
        winl += idx + 1
        acc = 10 * acc + d
    return acc

gold = silver = 0
for x in X:
    vs = list(map(lambda v: ord(v) - ord("0"), x))
    silver += find_largest(vs, 2)
    gold += find_largest(vs, 12)

print(silver, gold, sep="\n")
