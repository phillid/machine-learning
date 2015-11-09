#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <stdbool.h>

struct window {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *surface;
	unsigned int height;
	unsigned int width;
	char* title;
};

int display_init(struct window *wobj);
void display_stop(struct window *wobj);


#endif
