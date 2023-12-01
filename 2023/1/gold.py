#!/usr/bin/env python3

literals = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]


def f(line):
    low, high = 0, 0
    for i, c in enumerate(line):
        o = ord(c)
        if o <= ord("9") and o >= ord("0"):
            low = o - ord("0")
            break
        for j, lit in enumerate(literals, 1):
            if line[i:].startswith(lit):
                low = j
                break
        if low:
            break

    for i, c in enumerate(line[i:], i):
        o = ord(c)
        if o <= ord("9") and o >= ord("0"):
            high = o - ord("0")
        for j, lit in enumerate(literals, 1):
            if line[i:].startswith(lit):
                high = j
    return low * 10 + high


with open(0) as fp:
    print(sum(map(f, fp.readlines())))
