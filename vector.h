#ifndef VECTOR_H
#define VECTOR_H

typedef double vector_value_type;

struct vector
{
	unsigned long dimensions;
	vector_value_type *values;
	char *label;
};

struct vector *vector_new(unsigned long dimensions);
void vector_normalise(struct vector *v);
double vector_error(const struct vector *v1, const struct vector *v2);
void vector_random_values(struct vector *v, long min, long max);

#endif
