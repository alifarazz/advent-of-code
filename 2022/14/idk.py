#!/usr/bin/env python3

with open(0) as fp:
    lines = fp.read()
lines = lines.split()
lines = filter(lambda x: x != "->", lines)
tuples = map(lambda tu: tuple(map(lambda x: int(x), tu.split(','))), lines)
xy = list(zip(*tuples))
imax, imin = list(map(lambda x: x(x(xy[0]), 500), [max, min]))
jmax, jmin = list(map(lambda x: x(x(xy[1]), 0), [max, ])) + [0]
print(f'i:\tmax = {imax: 4}\tmin = {imin: 4}\t->\tMAX_DIM0 = {imax - imin + 1:4}')
print(f'j:\tmax = {jmax: 4}\tmin = {jmin: 4}\t->\tMAX_DIM1 = {jmax - jmin + 1:4}')

print(f'MAX_W = {imax: 4}', f'MAX_H = {jmax: 4}', sep='\n')
