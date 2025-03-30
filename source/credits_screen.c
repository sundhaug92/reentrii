#include "gamemanager.h"
#include <grrlib.h>
#include <wiiuse/wpad.h>

#include <stdio.h>

#include "credits1_png.h"
#include "credits_bg1_png.h"

GameModeExit credits_screen(GameState *global_state)
{
    GRRLIB_texImg *creditsBg1 = GRRLIB_LoadTexture(credits_bg1_png);
    GRRLIB_texImg *credits1 = GRRLIB_LoadTexture(credits1_png);

    if (creditsBg1 == NULL)
    {
        strcpy((*global_state).message[0], "Failed to load credits background 1");
        return (GameModeExit){.screen = SCREEN_ERROR};
    }

    if (credits1 == NULL)
    {
        strcpy((*global_state).message[0], "Failed to load credits image 1");
        return (GameModeExit){.screen = SCREEN_ERROR};
    }

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    int y = 480;

    while (true)
    {
        int bg_y = y <= 0 ? y / 2 : 0;
        GRRLIB_DrawImg(0, bg_y, creditsBg1, 0, 1, 1, RGBA(255, 255, 255, 255));
        GRRLIB_DrawImg(0, y, credits1, 0, 1, 1, RGBA(255, 255, 255, 255));

        GRRLIB_SetBlend(0);

        y -= 1;

        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if (pressed & WPAD_BUTTON_HOME)
        {
            break;
        }

        if (bg_y < -480)
        {
            printf("\n\nCredits screen done\n");
            printf("bg_y: %d\n", bg_y);
            printf("creditsBg1->h: %d\n", creditsBg1->h);
            printf("rmode->viHeight: %d\n", rmode->viHeight);
            break;
        }

        GRRLIB_Render();
    }
    GRRLIB_FreeTexture(creditsBg1);
    GRRLIB_FreeTexture(credits1);
    return (GameModeExit){.screen = SCREEN_SPLASH};
}