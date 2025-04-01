#include "gamemanager.h"
#include <grrlib.h>
#include <wiiuse/wpad.h>

#include <stdio.h>
#include <stdlib.h>

#include "orionmoon_png.h"

GameModeExit victory_screen(GameState *global_state)
{
    GRRLIB_texImg *bg_tex = GRRLIB_LoadTexture(orionmoon_png);

    char message[40][80];
    for (int line = 0; line < 40; line++)
        snprintf(message[line], 80, " ");

    if ((*global_state).cheatsEnabled)
        snprintf(message[0], 80, "With the help of your best cheating,");
    else
        snprintf(message[0], 80, "With the help of your great skill,");

    snprintf(message[1], 80, "you have defeated the evil doctor.");

    while (!(*global_state).exitRequested)
    {
        GRRLIB_DrawImg(0, 0, bg_tex, 0, 1, 1, RGBA(255, 255, 255, 255));
        GRRLIB_PrintfTTF(100, 50, (*global_state).basicFont, "Victory!", 32, 0x00FF00FF);

        WPAD_ScanPads();
        u32 held = WPAD_ButtonsHeld(0);
        GRRLIB_PrintfTTF(0, 480 - 24, (*global_state).basicFont, "Press A + B to continue", 16, 0x00FF00FF);

        for (int line = 0; line < 40; line++)
            GRRLIB_PrintfTTF(100, 100 + (line * 16), (*global_state).basicFont, message[line], 12, 0x00FF00FF);

        if ((held & WPAD_BUTTON_A) && (held & WPAD_BUTTON_B))
        {
            break;
        }
        GRRLIB_Render();
    }
    return (GameModeExit){.screen = SCREEN_CREDITS};
}