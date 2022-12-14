#!/usr/bin/env python3

from operator import mul
from functools import cmp_to_key, reduce
from itertools import islice
import json

# Look Ma! No for loops! (At least, not explicitly)


def LazyReadlines(fp):
    while line := fp.readline():
        yield line


def ParseInput(Input):
    return map(json.loads,
               filter(lambda x: x != "\n",
                      Input))


def PacketCmp(p1, p2):
    IsInt = lambda x: isinstance(x, int)
    if IsInt(p1) and IsInt(p2):
        return p2 - p1
    EncloseInListIfNeccessary = lambda x: [x] if IsInt(x) else x
    n1, n2 = map(EncloseInListIfNeccessary, [p1, p2])
    RecurseResult = map(lambda x: PacketCmp(*x), zip(n1, n2))
    FirstOkResult = next(filter(lambda x: x != 0,
                                RecurseResult),
                         None)
    return FirstOkResult or len(n2) - len(n1)


def Silver(Input):
    Score = lambda PairNumber, CmpResult: (CmpResult > 0) * PairNumber

    PacketPairs = zip(islice(Input, 0, None, 2),
                      islice(Input, 1, None, 2))
    Scores = map(lambda x: Score(x[0], PacketCmp(*x[1])),
                 enumerate(PacketPairs, 1))
    return sum(Scores)


def Gold(Input):
    def CheckPacketIfBeforeDecoders(Pkt):
        Before2 = PacketCmp([2], Pkt) < 0
        return (
            Before2,
            not Before2 and PacketCmp([6], Pkt) < 0
        )

    Position2, Position6 = reduce(
        lambda x, y: (x[0] + y[0], x[1] + y[1]),
        map(CheckPacketIfBeforeDecoders, Input),
        (1, 1),
    )
    return Position2 * (Position2 + Position6)


def main():
    with open(0) as fp:
        LazyLines = LazyReadlines(fp)
        Input = ParseInput(LazyLines)
        Input = list(Input)
    print(Silver(Input))
    print(Gold(Input))


if __name__ == "__main__":
    main()
