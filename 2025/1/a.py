#!/usr/bin/env python

fp = open(0, 'r')
X = fp.read().split()

gold = silver = 0
pos = 50

for x in X:
    ticks = ([1, -1][x[0] == 'L'] * int(x[1:]))
    new_pos = pos + ticks
    warp_count = abs(new_pos) // 100
    passed_0 = (pos > 0 and new_pos <= 0)
    gold += warp_count + passed_0
    pos = (new_pos) % 100
    silver += (pos == 0)
    
print(silver, gold, sep='\n')
