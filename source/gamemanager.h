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

typedef struct GameState
{
    GRRLIB_ttfFont* basicFont;
    int frameCount;
    char message[40][80];
    bool cheatsEnabled;
    bool exitRequested;
} GameState;

extern GameModeExit error_screen(GameState* global_state);
extern GameModeExit splash_screen(GameState* global_state);
extern GameModeExit credits_screen();

#endif