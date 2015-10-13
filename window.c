#include "window.h"

#include <SDL.h>
#include <stdbool.h>

int display_init(struct window *wobj, bool use_renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr,"Couldn't init SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}


	/* Create run-of-the-mill window at specified size */
	wobj->window = SDL_CreateWindow(wobj->title,
	                                SDL_WINDOWPOS_UNDEFINED,
	                                SDL_WINDOWPOS_UNDEFINED,
	                                wobj->width,
	                                wobj->height,
	                                SDL_WINDOW_SHOWN);
	if (wobj->window == NULL)
	{
		fprintf(stderr, "Couldn't create window: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (!use_renderer)
	{
		/* Grab window surface */
		wobj->surface = SDL_GetWindowSurface(wobj->window);
		if (wobj->surface == NULL)
		{
			fprintf(stderr, "Couldn't grab window surface: %s\n", SDL_GetError());
			return EXIT_FAILURE;
		}
		/* Clear/blank surface with grey, and update it */
		SDL_FillRect(wobj->surface,
					 NULL,
					 SDL_MapRGB(wobj->surface->format,
								0x33, 0x33, 0x33));
		SDL_UpdateWindowSurface(wobj->window);
	} else {
		/* create a renderer for the window too */
		wobj->renderer = SDL_CreateRenderer(wobj->window, -1, 0);
		if (wobj->renderer == NULL)
		{
			fprintf(stderr, "Couldn't grab window renderer: %s\n", SDL_GetError());
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}


void display_stop(struct window *wobj)
{
	SDL_DestroyWindow(wobj->window);
	SDL_Quit();
}
