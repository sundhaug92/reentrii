#include "gamemanager.h"
#include <grrlib.h>
#include <wiiuse/wpad.h>

#include <stdio.h>

#include "credits_bg1_png.h"

char story[][80] = {
    "The evil Doctor Lentokentta has",
    "stolen the retro-pack of the elders",
    "and is using it to power his",
    "time machine. He plans to go back",
    "in time and prevent the creation",
    "of the space-shuttle",
    "",
    "Your mission, should you choose to",
    "accept it, is to stop him and",
    "return the retro-pack to the elders",
    "before it's too late.",
    "",
    "Good luck, commander."
};

GameModeExit story_screen(GameState* global_state) {
    GRRLIB_texImg *storyBg = GRRLIB_LoadTexture(credits_bg1_png);
    GRRLIB_texImg *storyTex = GRRLIB_CreateEmptyTexture(640, 32 * sizeof(story) / sizeof(story[0]));

    GRRLIB_CompoStart();
    for (int i = 0; i < sizeof(story) / sizeof(story[0]); i++)
    {
        GRRLIB_PrintfTTF(0, i * 24, (*global_state).basicFont, story[i], 24, 0xFFFFFFFF);
    }
    GRRLIB_CompoEnd(0, 0, storyTex);

    if(storyBg == NULL) {
        strcpy((*global_state).message[0], "Failed to load story background 1");
        return (GameModeExit) { .screen = SCREEN_ERROR };
    }

    if(storyTex == NULL) {
        strcpy((*global_state).message[0], "Failed to load story image 1");
        return (GameModeExit) { .screen = SCREEN_ERROR };
    }

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);


    int y = 480;


    for (int frame = 0; frame < 60 * 20; frame++)
    {
        int bg_y = y <= 0 ? y / 2 : 0;
        GRRLIB_DrawImg(0, bg_y, storyBg, 0, 1, 1, RGBA(255, 255, 255, 255));
        GRRLIB_DrawImg(0, y, storyTex, 0, 1, 1, RGBA(255, 255, 255, 255));

        GRRLIB_SetBlend(0);

        y -= 1;

        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if(pressed & WPAD_BUTTON_HOME) {
            break;
        }

        GRRLIB_Render();
    }
    GRRLIB_FreeTexture(storyBg);
    return (GameModeExit) { .screen = SCREEN_GAME };
}