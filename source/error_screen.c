#include "gamemanager.h"
#include <grrlib.h>
#include <wiiuse/wpad.h>

#include <stdio.h>
#include <stdlib.h>

GameModeExit error_screen(GameState* state) {
    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    
    for (int line = 0; line < 40; line++)
        if(strlen((*state).message[line]) != 0)
            printf("%s\n", (*state).message[line]);
        

    while(true) {
        GRRLIB_PrintfTTF(100, 50, (*state).basicFont, "Error", 32, 0xFFFFFFFF);

        for (int line = 0; line < 40; line++)
            GRRLIB_PrintfTTF(100, 100 + (line * 16), (*state).basicFont, (*state).message[line], 12, 0xFFFFFFFF);

        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if((pressed & WPAD_BUTTON_A) && (pressed & WPAD_BUTTON_B)) {
            break;
        }
        GRRLIB_Render();
    }
    return (GameModeExit) { .screen = SCREEN_EXIT };
}