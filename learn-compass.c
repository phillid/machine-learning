#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

#include "window.h"
#include "vector.h"

#define PIXELS(s) ((uint32_t*)(s->pixels))

#define TRAINING_SIZE 40

void train();
void plot(struct vector *v);

/* to do: make non-global */
struct vector *data[TRAINING_SIZE];
struct window wind = {0};


int main()
{
	SDL_Event e = {0};
	bool running = false;
	bool new = false;

	/* Start SDL window */
	wind.title = "Simple machine learning of the compass";
	wind.width = 600;
	wind.height = 600;
	display_init(&wind, true);
	int i = 0;

	train();

	running = true;
	new = true;
	while (running)
	{
		if (new)
		{
			struct vector *v = vector_new(2);
			double error = 0;
			/* choose a random vector */
			v->values[0] = (int)(wind.width/2 - rand()%wind.width);
			v->values[1] = (int)(wind.height/2 - rand()%wind.height);
			plot(v);
			vector_normalise(v);
			double min = vector_error(data[0], v);
			v->label = data[0]->label;
			for (i = 0; i < TRAINING_SIZE; i++)
			{
				error = vector_error(data[i], v);
				if (error < min)
				{
					min = error;
					v->label = data[i]->label;
				}
			}
			printf("I think this is %s\n", v->label);
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
	return 0;
}

void plot(struct vector *v)
{
	/* assert that the vector has exactly two dimensions */
	if (v->dimensions != 2)
	{
		fprintf(stderr, "plot: need a 2-D vector, got a %lu-D one\n", v->dimensions);
		return;
	}

	SDL_SetRenderDrawColor(wind.renderer, 0, 0, 0, 255);
	SDL_RenderClear(wind.renderer);

	SDL_SetRenderDrawColor(wind.renderer, 255, 255, 255, 255);
	/* to do: break down or self-document this */
	SDL_RenderDrawLine(wind.renderer, wind.width/2, wind.height/2, wind.width/2+v->values[0], wind.height/2+v->values[1]);

	SDL_RenderPresent(wind.renderer);
}

void train()
{
	int i = 0;
	struct vector *v = NULL;
	char buffer[1024];


	for (i = 0; i < TRAINING_SIZE; i++)
	{
		v = vector_new(2);
		/* choose a random vector */
		v->values[0] = (int)(wind.width/2 - rand()%wind.width);
		v->values[1] = (int)(wind.height/2 - rand()%wind.height);

		plot(v);
		//SDL_UpdateWindowSurface(wind.window);

		printf("\nLabel: ");
		if (!fgets(buffer, sizeof(buffer), stdin))
		{
			fprintf(stderr, "Failed to get line from stdin");
			return EXIT_FAILURE;
		}
		v->label = strdup(buffer);
		vector_normalise(v);
		data[i] = v;
	}
}
