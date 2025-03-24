#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <grrlib.h>


enum screens
{
    SCREEN_ERROR = -2,
    SCREEN_EXIT = -1,
    SCREEN_SPLASH = 0,
    SCREEN_CREDITS,
    SCREEN_GAME
};

typedef struct GameModeExit
{
    int screen;
} GameModeExit;


// TODO: Make a extern struct
typedef struct GameState
{
    GRRLIB_ttfFont* basicFont;
    int frameCount;
    char message[40][80];
    bool cheatsEnabled;
} GameState;

extern GameModeExit error_screen(GameState* state);
extern GameModeExit splash_screen(GameState* state);
extern GameModeExit credits_screen();

#endif