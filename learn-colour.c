#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

#include "window.h"
#include "vector.h"

#define TRAINING_SIZE 10

int train();
void show_off(struct vector **data, size_t data_size);
void plot(struct vector *v);

/* to do: make non-global */
struct vector *data[TRAINING_SIZE];
struct window wind;

int main(void)
{
	SDL_Event e = {0};
	bool running = false;
	int i = 0;

	/* Start SDL window */
	wind.title = "Simple machine learning of colours";
	wind.width = 600;
	wind.height = 600;
	if (display_init(&wind) == EXIT_FAILURE)
		return EXIT_FAILURE;

	if (train() == EXIT_FAILURE)
		return EXIT_FAILURE;


	running = true;
	show_off(data, TRAINING_SIZE);
	while (running)
	{
		SDL_WaitEvent(&e);
		switch (e.type)
		{
			case SDL_QUIT:
				fprintf(stderr, "quit\n");
				running = false;
				break;

			case SDL_KEYDOWN:
				fprintf(stderr, "key down\n");
				switch (e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
					case SDLK_q:
						running = false;
						break;
				}
				show_off(data, TRAINING_SIZE);
				break;
			default:
				break;
		}
	}

	display_stop(&wind);

	for (i = 0; i < TRAINING_SIZE; i++)
		vector_destroy(data[i]);

	return 0;
}


void plot(struct vector *v)
{
	/* assert that the vector has correct dimensions */
	if (v->dimensions != 3)
	{
		fprintf(stderr, "plot: need a 3-D vector, got a %lu-D one\n", v->dimensions);
		return;
	}

	/* Clear/blank surface with grey, and update it */
	SDL_FillRect(wind.surface,
				 NULL,
				 SDL_MapRGB(wind.surface->format,
				            v->values[0], v->values[1], v->values[2]));
	SDL_UpdateWindowSurface(wind.window);
}

int train()
{
	int i = 0;
	struct vector *v = NULL;
	char buffer[1024];


	for (i = 0; i < TRAINING_SIZE; i++)
	{
		v = vector_new(3);
		vector_random_values(v, 0, 255);

		plot(v);

		printf("\nLabel: ");
		if (!fgets(buffer, sizeof(buffer), stdin))
		{
			fprintf(stderr, "Failed to get line from stdin");
			return EXIT_FAILURE;
		}
		v->label = strdup(buffer);
		data[i] = v;
	}
	return EXIT_SUCCESS;
}

void show_off(struct vector **data, size_t data_size)
{
	struct vector *v = NULL;
	struct vector *closest = NULL;

	v = vector_new(3);
	/* choose a random vector */
	vector_random_values(v, 0, 255);
	plot(v);
	closest = vector_closest(data, data_size, v);
	printf("I think this is %s\n", closest->label);
	vector_destroy(v);
}
