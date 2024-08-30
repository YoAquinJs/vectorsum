"""Generates a file with random vectors"""

import argparse
import math
from random import uniform

description = "Script for generating a file with random 2D vectors"
parser = argparse.ArgumentParser(description=description)

parser.add_argument("-f", "--filename", type=str, default="vectors.txt",
                    help="The name of the output file")
parser.add_argument("-c", "--vector-count", type=int, default=100,
                    help="The number of vectors to store")
parser.add_argument("-d", "--degrees", action="store_true", default=False,
                    help="Use degrees instead of radians")

def main() -> None:

    args = parser.parse_args()

    random_degree = lambda: round(uniform(1, 360), 2)
    random_rad = lambda: round(uniform(0, math.pi*2), 4)
    gen_direction = random_degree if args.degrees else random_rad

    vectors = []
    for _ in range(args.vector_count):
        magnitude = round(uniform(0, 100), 2)
        direction = gen_direction()
        vectors.append(f"{magnitude}\t{direction}")

    with open(args.filename, "w") as file:
        file.write("\n".join(vectors))
        file.write("\n")

if __name__ == "__main__":
    main()
