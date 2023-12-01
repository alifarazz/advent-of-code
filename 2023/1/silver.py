#!/usr/bin/env python3


def f(line):
    ret = 0
    for c in line:
        o = ord(c)
        if o <= ord("9") and o >= ord("0"):
            ret = o - ord("0")
            break
    for c in reversed(line):
        o = ord(c)
        if o <= ord("9") and o >= ord("0"):
            ret *= 10
            ret += o - ord("0")
            break
    return ret


with open(0) as fp:
    print(sum(map(f, fp.readlines())))
