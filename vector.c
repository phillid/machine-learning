#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "vector.h"


struct vector *vector_new(unsigned long dimensions)
{
	struct vector *v = NULL;

	v = malloc(sizeof(struct vector));

	if (!v)
		return NULL;

	v->values = malloc(dimensions * sizeof(vector_value_type));
	
	if (!v->values)
	{
		free(v);
		return NULL;
	}
	v->dimensions = dimensions;
	return v;
}

/* scaled vector to normalise along first dimension */
void vector_normalise(struct vector *v)
{
	unsigned long i = 0;
	double scale = 0;

	scale = abs(v->values[0]);

	for (i = 0; i < v->dimensions; i++)
		v->values[i] /= scale;
}

double vector_error(const struct vector *v1, const struct vector *v2)
{
	unsigned long i = 0;
	double running = 0.f;
	if (v1->dimensions != v2->dimensions)
		return NAN;

	/* build up running total of x^2 + y^2 + z^2 ... */
	for (i = 0, running = 0.f; i < v1->dimensions; i++)
		running += pow(v2->values[i] - v1->values[i], 2);

	return sqrt(running);
}

/* fill existing vector with random values between min and max */
void vector_random_values(struct vector *v, double min, double max)
{
	unsigned long i;
	/* FIXME check if need to use drand */
	for (i = 0; i < v->dimensions; i++)
	{
		v->values[i] = min + rand()%(max - min)
	}
}
