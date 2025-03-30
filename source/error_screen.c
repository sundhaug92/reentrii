#include "gamemanager.h"
#include <grrlib.h>
#include <wiiuse/wpad.h>

#include <stdio.h>
#include <stdlib.h>

GameModeExit error_screen(GameState* global_state) {
    GRRLIB_SetBackgroundColour(0x00, 0x00, 0xFF, 0xFF);

    
    for (int line = 0; line < 40; line++)
        if(strlen((*global_state).message[line]) != 0)
            printf("%s\n", (*global_state).message[line]);
        

    while(!(*global_state).exitRequested) {
        GRRLIB_PrintfTTF(100, 50, (*global_state).basicFont, "Error", 32, 0xFFFFFFFF);

        for (int line = 0; line < 40; line++)
            GRRLIB_PrintfTTF(100, 100 + (line * 16), (*global_state).basicFont, (*global_state).message[line], 12, 0xFFFFFFFF);

        WPAD_ScanPads();
        u32 held = WPAD_ButtonsHeld(0);
        if((held & WPAD_BUTTON_A) && (held & WPAD_BUTTON_B)) {
            break;
        }
        GRRLIB_Render();
    }
    return (GameModeExit) { .screen = SCREEN_EXIT };
}