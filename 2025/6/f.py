#!/usr/bin/env python

from functools import reduce
from itertools import chain, pairwise
import operator

fp = open(0, "r")
lines = fp.read().split("\n")

ops_raw = lines[-2]
vals_raw = lines[:-2]
ops = ops_raw.split()

silver = 0
vals = zip(*map(lambda l: l.split(), vals_raw))
for op, vs in zip(ops, vals):
    op_f = [operator.add, operator.mul][op == "*"]
    silver += reduce(op_f, map(int, vs))
print(silver)

op_w_idx = chain(filter(lambda x: x[1] != " ", enumerate(ops_raw)),
                 [(len(ops_raw) + 1, "X")])
gold = 0
for (col_idx, op), (col_idx_end, _) in pairwise(op_w_idx):
    op_f = [operator.add, operator.mul][op == "*"]
    section_acc = int(op == "*")
    for ci in range(col_idx, col_idx_end - 1):
        num = 0
        for row in vals_raw:
            c = row[ci]
            if num == 0 and c == " ":
                continue
            elif num != 0 and c == " ":
                break
            else:
                num = num * 10 + ord(c) - ord("0")
        section_acc = op_f(section_acc, num)
    gold += section_acc

print(gold)
