#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#define SETTINGS_FILE "options.ini"

#include <stdio.h>

#include <SDL2/SDL.h>

typedef struct
{
    int width;
    int height;
    SDL_WindowFlags window_flags;
} Settings;

char loadSettings(char* path, Settings* s);
char writeSettings(Settings* s);
#endif // SETTINGS_H_INCLUDED
