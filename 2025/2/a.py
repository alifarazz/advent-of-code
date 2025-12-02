#!/usr/bin/env python

import re

fp = open(0, 'r')
X = fp.read().split(',')

gold = silver = 0
gold_re = r'^([0-9]+)\1+$'

for x in X:
    a, b = [int(v) for v in x.split('-')]
    for i in range(a, b + 1):
        s = str(i)
        m = len(s) // 2
        silver += i * (s[:m] == s[m:])
        gold += i * (re.match(gold_re, s) is not None)

print(silver, gold, sep='\n')
