#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "vector.h"


/* create new vector of specified dimensions */
struct vector *vector_new(unsigned long dimensions)
{
	struct vector *v = NULL;

	v = malloc(sizeof(struct vector));

	if (!v)
		return NULL;

	v->values = malloc(dimensions * sizeof(double));

	if (!v->values)
	{
		free(v);
		return NULL;
	}
	v->dimensions = dimensions;
	return v;
}


/* destroy a vector allocated with vector_new
 * free()s the value array and finally frees the vector
 */
void vector_destroy(struct vector *v)
{
	free(v->values);
	free(v);
}


/* scale vector to normalise along first dimension */
void vector_normalise(struct vector *v)
{
	unsigned long i = 0;
	double scale = 0;

	scale = fabs(v->values[0]);

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

void vector_average(struct vector *v, struct vector **vs, const unsigned long count)
{
	unsigned long i = 0;
	unsigned long dim = 0;

	/* FIXME: doesn't check that dimensions line up */

	for (dim = 0; dim < v->dimensions; dim++)
	{
		v->values[dim] = 0.f;
		for (i = 0; i < count; i++)
			v->values[dim] += vs[i]->values[dim];

		v->values[dim] /= i;
	}
}

/* fill existing vector with random values between min and max */
void vector_random_values(struct vector *v, long min, long max)
{
	unsigned long i;
	/* FIXME check if need to use drand */
	for (i = 0; i < v->dimensions; i++)
		v->values[i] = min + rand()%(int)(max - min);
}


/* find vector in array of vectors with least error from another vector */
struct vector* vector_closest(struct vector **vectors, size_t size, struct vector *vector)
{
	unsigned long i;
	double min_error = 0;
	double error = 0;
	struct vector *result = NULL;

	/* preload a valid error value and result */
	result = vector;
	min_error = vector_error(vectors[0], result);

	/* find vector in `vectors` with lowest error from `vector` */
	for (i = 0; i < size; i++)
	{
		error = vector_error(vectors[i], result);
		if (error < min_error)
		{
			min_error = error;
			result = vectors[i];
		}
	}
	return result;
}


