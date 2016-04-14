struct vector
{
	unsigned long dimensions;
	double *values;
	char *label;
};

struct vector *vector_new(unsigned long dimensions);
void vector_destroy(struct vector *v);
void vector_normalise(struct vector *v);
double vector_error(const struct vector *v1, const struct vector *v2);
int vector_average(struct vector *v, struct vector **vs, const unsigned long count);
void vector_random_values(struct vector *v, long min, long max);
struct vector* vector_closest(struct vector **vectors, size_t size, struct vector *vector);
