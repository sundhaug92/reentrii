#include "gamemanager.h"
#include <grrlib.h>
#include <wiiuse/wpad.h>

#include <stdio.h>
#include <stdlib.h>

GameModeExit gameover_screen(GameState *global_state)
{
    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    char message[40][80];
    
    for (int line = 0; line < 40; line++)
        snprintf(message[line], 80, " ");

    #ifdef DEBUG
    printf("GO: Entering gameover_screen\n");
    #endif

    if ((*global_state).cheatsEnabled)
        snprintf(message[0], 80, "Despite your best cheating,");
    else
        snprintf(message[0], 80, "Despite your best efforts,");

    if ((*global_state).level == 1)
    {
        snprintf(message[1], 80, "you have failed to defeat the evil doctor.");
        snprintf(message[2], 80, " ");
    }
    else
    {
        snprintf(message[1], 80, "despite fighting them across %d levels,", (*global_state).level - 1);
        snprintf(message[2], 80, "you've failed to defeat the evil doctor.");
    }

    snprintf(message[3], 80, "Better luck next time.");

    #ifdef DEBUG
    printf("GO: Message:\n");
    for (int line = 0; line < 40; line++)
    {
        printf("GO: %s\n", message[line]);
    }
    #endif

    while (!(*global_state).exitRequested)
    {
        GRRLIB_PrintfTTF(100, 50, (*global_state).basicFont, "GAME OVER", 32, 0x00FF00FF);

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