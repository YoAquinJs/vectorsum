#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "arg-parse.h"

#undef calculate
#undef getrusage

#define PI 3.1415926535897932384626433

typedef struct vector{
    double mag;
    double dir;
}vector;

void sumv2tov1(vector* v1, vector* v2);
double deg_to_rad(double deg);

const char *program_version = "vectorsum 1.0";
const char doc[] = "C program for summing vectors\n"
                    "input must be formatted by (double) (double) newline\n"
                    "stops on the first unformatted or empty line\n";

int main(int argc, char **argv) {
    // parse arguments
    Arguments arguments = get_default_arguments();
    struct argp argp_config = argp_conf();
    argp_parse(&argp_config, argc, argv, 0x0, NULL, &arguments);

    // open file if specified
    FILE* input = stdin;
    if (arguments.filename){
        input = fopen(arguments.filename, "r");
        if (!input){
            fprintf(stderr, "Could not open %s.\n", arguments.filename);
            exit(EXIT_FAILURE);
        }
    }

    vector vToSum, result={0,0};

    int reads = 0;
    double mag, dir;
    char *line = NULL;
    size_t len = 0;
    int read;
    while ((read = getline(&line, &len, input)) != -1){
        if (sscanf(line, "%lf %lf", &mag, &dir) != 2)
             break;

        reads++;
        vToSum.mag = mag;
        vToSum.dir = arguments.rads ? dir : deg_to_rad(dir);
        sumv2tov1(&result, &vToSum);
    }
    free(line);

    if (reads < 2){
        printf("must provide at least two vectors");
        exit(EXIT_FAILURE);
    }

    printf("result: %.*lf %.*lf\n", arguments.precision, result.mag,
                                    arguments.precision, result.dir);

    if (arguments.filename)
        fclose(input);
    exit(EXIT_SUCCESS);
}

void sumv2tov1(vector* v1, vector* v2){
    if (v2->mag == 0)
        return;

    if (v1->mag == 0){
        v1->mag = v2->mag;
        v1->dir = v2->dir;
        return;
    }

    //Angle formed by intersection of the two vectors or triangule angle opposite of resulting vector
    //Compute 180 +-(vector1 angle - vector2 angle)
    double trigAngleX = (v1->dir > v2->dir) ? (PI - v1->dir + v2->dir) : (PI + v1->dir - v2->dir);
    //Compute resulting magnitude by cosine law
    double magnitudeX = sqrt(pow(v1->mag, 2) + pow(v2->mag, 2) - (2*v1->mag*v2->mag*cos(trigAngleX)));

    if (magnitudeX == 0){
        v1->mag = magnitudeX;
        v1->dir = 0;
        return;
    }

    //Angle formed by intersection of initial vector and resulting vector or triangule angle opposite of vector 2
    double trigAngleV2 = acos((pow(magnitudeX, 2) + pow(v1->mag, 2) - pow(v2->mag, 2)) / (2 * magnitudeX * v1->mag));
    //Compute resulting vector angle by +- initial vector angle in the two cases
    double angleX = v1->dir + (v1->dir > v2->dir ? (trigAngleV2 * -1) : trigAngleV2);

    // printf("%lf %lf %lf %lf\n", trigAngleX, magnitudeX, trigAngleV2, angleX);
    v1->dir = angleX;
    v1->mag = magnitudeX;
}

double deg_to_rad(double deg){
    return deg * (PI/(double)180);
}
