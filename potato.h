#ifndef POTATO_H_INCLUDED
#define POTATO_H_INCLUDED

#include <stdio.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* screen;

char quit;

#include "settings.h"
#include "sMachine.h"

#endif // POTATO_H_INCLUDED
