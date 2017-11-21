#include "settings.h"

char loadSettings(char* path, Settings* s)
{
    FILE* f;
    f =  fopen(path, "r");
    fread((void*)s, sizeof(Settings), 1, f);

    fclose(f);
    return 1;
};

char writeSettings(Settings* s)
{
    FILE* f = fopen(SETTINGS_FILE, "w");
    fwrite((const void*)s, sizeof(Settings), 1, f);
    fclose(f);

    return 1;
};
