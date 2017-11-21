#include "potato.h"

#include "./GUI/gui.h"

Settings settings;
sMachine sm;

int main()
{
    printf("Starting Potato Farmer 2017.\n");
    printf("\tLoading settings.\n");

    settings.width = 800;
    settings.height = 600;
    settings.window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS;

    writeSettings(&settings);
    settings.width = 0;
    settings.height = 0;
    settings.window_flags = 0;

    loadSettings(SETTINGS_FILE, &settings);
    printf("\tResolution:%dx%d;\n\tfullscreen: 0x%08x;\n", settings.width, settings.height, settings.window_flags);

    if (initSDL(&settings))
    {
        printf("Exiting\n");
        return 1;
    };

    if (initSM(&sm))
    {
        printf("Exiting\n");
        return 1;
    };

    mainLoop(&sm);

    return 0;
}
