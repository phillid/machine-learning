#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

#include "window.h"
#include "vector.h"

#define PIXELS(s) ((uint32_t*)(s->pixels))

#define TRAINING_SIZE 60

int train();
void plot(struct vector *v);

/* to do: make non-global */
struct vector *data[TRAINING_SIZE];
struct window wind;

int main()
{
	SDL_Event e = {0};
	bool running = false;
	bool new = false;
	int i = 0;
	struct vector *v = NULL;
	double error = 0;
	double min_error = 0;

	/* Start SDL window */
	wind.title = "Simple machine learning of colours";
	wind.width = 600;
	wind.height = 600;
	if (display_init(&wind) == EXIT_FAILURE)
		return EXIT_FAILURE;
	
	if (train() == EXIT_FAILURE)
		return EXIT_FAILURE;

	running = true;
	new = true;
	while (running)
	{
		if (new)
		{
			v = vector_new(3);
			/* choose a random vector */
			vector_random_values(v, 0, 255);
			plot(v);
			vector_normalise(v);
			/* FIXME: be separate func */
			min_error = vector_error(data[0], v);
			for (i = 0; i < TRAINING_SIZE; i++)
			{
				error = vector_error(data[i], v);
				if (error < min_error)
				{
					min_error = error;
					v->label = data[i]->label;
				}
			}
			printf("I think this is %s\n", v->label);
			vector_destroy(v);
			new = false;
		}
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
				new = true;
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
				 SDL_MapHSV(wind.surface->format,
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
