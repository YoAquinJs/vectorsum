#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>

#undef calculate
#undef getrusage

#define PI 3.1415926535897932384626433

// Vector linked-list node
typedef struct vectornode
{
    double magnitude;
    double angle;
	struct vectornode *next;
}
vectornode;

// Vector definition
typedef struct vector
{
    double magnitude;
    double angle;
}
vector;

// Calculate vector sum
vector sumvectors(vector v1, vector v2, int i, int floatPrecision, bool isRads);
// Convert vectornode to vector struct
vector vnodetovector(vectornode *node);
// Calculate runtime
double calculate(const struct rusage *b, const struct rusage *a);

int main(int argc, char *argv[])
{
	// Time calc setup
    struct rusage before, after;
    double runtime = 0.0;
    getrusage(RUSAGE_SELF, &before);

	// Result values decimal places (default 3)
	int floatPrecision = 3;
	// Input and result angle values type (default grads)
	bool isRads = false;

    FILE *finpt = NULL;

	// Get command arguments
	if (argc > 1){// Angle value type
		if (strcmp(argv[1], "rads") == 0){
			isRads = true;
		}else if (strcmp(argv[1], "grads") == 0){
			isRads = false;
		}else{
        	printf("must provide 'rads' or 'grads' in isRad\n");
			return 1; // Bad Input
		}

		if (argc > 2){// Float precision
			floatPrecision = atoi(argv[2]);
			if (floatPrecision < 0){
        		printf("must provide positive integer or zero in floatingPrecision");
				return 1; // Bad Input
			}

			if (argc > 3){// Vector read file
				finpt = fopen(argv[3], "r");
    			if (finpt == NULL){
        			printf("Could not open %s.\n", argv[2]);
					return 2;
	    		}
			}
		}
	}

	vector result;

	// Throught file input
	if (finpt != NULL){
		// Vector linked-list
		vectornode *vectors = NULL;
		// Holder for linked-list insert and unload
		vectornode *holder = NULL;

		// Values readed from input file (i+1)
		int i = -1;
		// Read value buffer
		double *buff = malloc(sizeof(double));
	    while (fscanf(finpt, "%lf", buff) > 0){// Format input to double and copy to buffer, if unvalid or reached the end stop
			i++;

			// Every two valus insert new vector
			if (i % 2 == 0){
				holder = vectors;
				// Allocate new vector in memory
				vectors = malloc(sizeof(vectornode));
				// Insert it at the beginning of the list
				vectors->next = holder;
				vectors->magnitude = *buff;
			}else{
				vectors->angle = *buff * (isRads ? 1 : PI / 180);
			}
		}

		// Ensure enough reads for two vectors
		if (i < 3){
			printf("must provide at least 2 vectors");

			// Free all memory allocated
			fclose(finpt);
		    free(buff);
			for (vectornode *node = vectors; node != NULL; node = holder){
				holder = node->next;
				free(node);
			}

			return 1; // Bad Input
		}

		// Sums first two vectors
		result = sumvectors(vnodetovector(vectors), vnodetovector(vectors->next), 1, floatPrecision, isRads);
		int j = 2;
		for (vectornode *node = vectors->next->next; node != NULL; node = node->next){// Iterate throught linked-list
			// Sums the past resultant vector and the next one until no more vectors
			result = sumvectors(result, vnodetovector(node), j, floatPrecision, isRads);
			j++;
		}

		// Display results
		printf("Result magnitude: %.*lf\n", floatPrecision, result.magnitude);
		printf("Result angle: %.*lf %s\n", floatPrecision, result.angle * (isRads ? 1 : 180 / PI), isRads ? "rads" : "grads");// Return result in input value
		printf("Operations: %i\n", j);

		// Display time
		getrusage(RUSAGE_SELF, &after);
    	runtime = calculate(&before, &after);
	    printf("Runtime: %f\n", runtime);

		// Free all memory allocated
		fclose(finpt);
	    free(buff);
		for (vectornode *node = vectors; node != NULL; node = holder){
			holder = node->next;
			free(node);
		}

		return 0;// Successful
	}

	// Throught command input

	int n = 0;

	printf("Number of Vectors: ");
	if (scanf("%i", &n) != 1){
		printf("must provide positive integer\n");
		return 1; // Bad Input
	}

	if (n < 2){
		printf("must provide at least two vectors\n");
		return 1; // Bad Input
	}

	// Allocate the amount of vectors in memory
	vector *vectors = malloc(sizeof(vector) * n);

	// First vector arguments
	printf("Vector 1 magnitude: ");
	if (scanf("%lf", &vectors[0].magnitude) != 1){
		printf("must provide numeric value\n");
		return 1; // Bad Input
	}
	printf("Vector 1 angle: ");
	if (scanf("%lf", &vectors[0].angle) != 1){
		printf("must provide numeric value\n");
		return 1; // Bad Input
	}
	// Ensure angles are calculated in rads
	vectors[0].angle = vectors[0].angle * (isRads ? 1 : PI / 180);

	for (int i = 1; i < n; i++)
	{
		// Ask for the rest of vectors
		printf("Vector %i magnitude: ", i+1);
		if (scanf("%lf", &vectors[i].magnitude) != 1){
			printf("must provide numeric value\n");
			return 1; // Bad Input
		}
		printf("Vector %i angle: ", i+1);
		if (scanf("%lf", &vectors[i].angle) != 1){
			printf("must provide numeric value\n");
			return 1; // Bad Input
		}
		// Ensure angles are calculated in rads
		vectors[i].angle = vectors[i].angle * (isRads ? 1 :PI / 180);

		// Sum first to vector
		if (i == 1){
			result = sumvectors(vectors[0], vectors[1], 1, floatPrecision, isRads);
		}else{
			// Sums the past resultant vector and the next one until no more vectors
			result = sumvectors(result, vectors[i], i, floatPrecision, isRads);
		}
	}

	// Display results
	printf("Result magnitude: %.*lf\n", floatPrecision, result.magnitude);
	printf("Result angle: %.*lf %s\n", floatPrecision, result.angle * (isRads ? 1 : 180 / PI), isRads ? "rads" : "grads"); // Return result in input value

	// Free memory allocated
    free(vectors);
	return 0; // Successful
}

vector sumvectors(vector v1, vector v2, int i, int floatPrecision, bool isRads){
	//Angle formed by intersection of the two vectors or triangule angle opposite of resulting vector
	//Compute 180 +-(vector1 angle - vector2 angle)
	double trigAngleX = (v1.angle > v2.angle) ? (PI - v1.angle + v2.angle) : (PI + v1.angle - v2.angle);
	//Compute resulting magnitude by cosine law
	double magnitudeX = sqrt(pow(v1.magnitude, 2) + pow(v2.magnitude, 2) - (2*v1.magnitude*v2.magnitude*cos(trigAngleX)));
	//Angle formed by intersection of initial vector and resulting vector or triangule angle opposite of vector 2
	double trigAngleV2 = acos((pow(magnitudeX, 2) + pow(v1.magnitude, 2) - pow(v2.magnitude, 2)) / (2 * magnitudeX * v1.magnitude));
	//Compute resulting vector angle by +- initial vector angle in the two cases
	double angleX = v1.angle + (v1.angle > v2.angle ? (trigAngleV2 * -1) : trigAngleV2);

	// Display essential calculations value
	printf("Sum(%i): triangule thetaX: %.*lf triangule thetaVector2: %.*lf magnitudeX: %.*lf thetaX: %.*lf\n",
		   i, floatPrecision, trigAngleX * (isRads ? 1 : 180 / PI), floatPrecision, trigAngleV2 * (isRads ? 1 : 180 / PI),
		   floatPrecision, magnitudeX, floatPrecision, angleX * (isRads ? 1 : 180 / PI));

	// Parse to vector struct
	vector result;
	result.magnitude = magnitudeX;
	result.angle = angleX;

	return result;
}

vector vnodetovector(vectornode *node){
	vector vec;
	vec.magnitude = node->magnitude;
	vec.angle = node->angle;

	return vec;
}

double calculate(const struct rusage *b, const struct rusage *a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                  (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                 ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                  (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}