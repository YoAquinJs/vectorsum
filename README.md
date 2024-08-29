# Vector Sum CLI

Vector sum program using trigonometry (cosine law) and analytic geometry, must provide always at least two vectors, two uses:
- Passing arguments via command line, insert number of vectors to sum and fill each vector magnitude and angle, args:
  **isRads** angle values input in radians *rads* or degrees *grads*
  **floatingPrecision** floating point decimal place, positive integer or 0
- Passing arguments by a text file, format data like "magnitude1 angle1 magnitude2 angle2..." separated by spaces
  **filename** file name containing vectors data

------------

The python script vector-gen serve to generate random vectors to a file for testing,
use -h for details on usage
```sh
# usage: vector-gen.py [-h] [-f FILENAME] [-c VECTOR_COUNT] [-d]
python vector-gen.py -f vectors.txt -c 1000
```
