from random import randrange as rnd
from sys import argv

with open(argv[1], 'w') as f:
    try:
        argv[2] = int(argv[2])
        for i in range(int(argv[2])):
            f.write(f"{rnd(1, 10)} {rnd(0, 360)}\n")
    except:
        print("unvalid number of vectors")