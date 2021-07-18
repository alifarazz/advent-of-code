from itertools import product
from math import prod

def pred(t) -> bool:
    if sum(t) == 2020:
        return True

def main():
    with open("input", 'r') as fp:
        arr = list(map(int, fp.readlines()))
        a1 = next(filter(pred, product(arr, arr)))
        a2 = next(filter(pred, product(arr, arr, arr)))
        print(prod(a1), prod(a2), sep='\n')

if __name__ == '__main__':
    main()
