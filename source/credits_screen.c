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

    while (!(*global_state).exitRequested)
    {
        int bg_y = y <= 0 ? y / 2 : 0;
        GRRLIB_DrawImg(0, bg_y, creditsBg1, 0, 1, 1, RGBA(255, 255, 255, 255));
        GRRLIB_DrawImg(0, y, credits1, 0, 1, 1, RGBA(255, 255, 255, 255));

        GRRLIB_SetBlend(0);

        if(y <= -480)
        {
            GRRLIB_PrintfTTF(100, 200 + (24*0), (*global_state).basicFont, "Original pictures of Orion/Agena", 16, 0xFFFFFFFF);
            GRRLIB_PrintfTTF(100, 200 + (24*1), (*global_state).basicFont, "National Air and Space Administration", 16, 0xFFFFFFFF);
            GRRLIB_PrintfTTF(100, 200 + (24*2), (*global_state).basicFont, "European Space Agency", 16, 0xFFFFFFFF);
            GRRLIB_PrintfTTF(100, 200 + (24*4), (*global_state).basicFont, "Original pictures of aeroplanes and ASM-135 ASAT", 16, 0xFFFFFFFF);
            GRRLIB_PrintfTTF(100, 200 + (24*5), (*global_state).basicFont, "United States Air Force", 16, 0xFFFFFFFF);
            GRRLIB_PrintfTTF(100, 200 + (24*7), (*global_state).basicFont, "Thanks to", 16, 0xFFFFFFFF);
            GRRLIB_PrintfTTF(100, 200 + (24*8), (*global_state).basicFont, "GRRLIB", 16, 0xFFFFFFFF);
            GRRLIB_PrintfTTF(100, 200 + (24*9), (*global_state).basicFont, "DevkitPPC", 16, 0xFFFFFFFF);

        }

        y -= 1;

        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if (pressed & WPAD_BUTTON_HOME)
        {
            break;
        }

        if (bg_y < -480)
        {
            #ifdef DEBUG
            printf("\n\nCredits screen done\n");
            printf("bg_y: %d\n", bg_y);
            printf("creditsBg1->h: %d\n", creditsBg1->h);
            printf("rmode->viHeight: %d\n", rmode->viHeight);
            #endif
            break;
        }

        GRRLIB_Render();
    }
    GRRLIB_FreeTexture(creditsBg1);
    GRRLIB_FreeTexture(credits1);
    return (GameModeExit){.screen = SCREEN_SPLASH};
}