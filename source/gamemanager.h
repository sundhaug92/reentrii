#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <grrlib.h>


enum screens
{
    SCREEN_ERROR = -2,
    SCREEN_EXIT = -1,
    SCREEN_SPLASH = 0,
    SCREEN_CREDITS,
    SCREEN_STORY,
    SCREEN_GAME,
    SCREEN_GAME_OVER,
    SCREEN_VICTORY
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

    // Game state
    int lives;
    int score;
    int level;
} GameState;

extern GameModeExit error_screen(GameState* global_state);
extern GameModeExit splash_screen(GameState* global_state);
extern GameModeExit game_screen(GameState* global_state);
extern GameModeExit credits_screen();
extern GameModeExit story_screen(GameState* global_state);

#endif