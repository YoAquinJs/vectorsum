# Vector Sum CLI
--------------

Vector sum script using trigonometry (cosine law) and analytic geometry, must provide always at least two vectors, two uses:
- Passing arguments via command line, insert number of vectors to sum and fill each vector magnitude and angle, args:
  **isRads** angle values input in radians *rads* or degrees *grads*
  **floatingPrecision** floating point decimal place, positive integer or 0
- Passing arguments by a text file, format data like "magnitude1 angle1 magnitude2 angle2..." separated by spaces
  **filename** file name containing vectors data
  
for generating a file with vectors
```
python vectors.py vectors.txt numberOfVectors
```
