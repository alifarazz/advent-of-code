#!/usr/bin/env python3
import random, io

rand = random.Random(69_420)  # set seed
length = 10_000_000
num_range = (0, 1 << 16 - 1)
with io.open('bigboy1.1.txt', 'w', buffering=1 << 14) as fp:
    for _ in range(length):
        num = rand.randint(*num_range)
        fp.write(f'{num}\n')
