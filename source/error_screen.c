#include "gamemanager.h"
#include <grrlib.h>
#include <wiiuse/wpad.h>

GameModeExit error_screen(GameState* state) {
    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);


    while(true) {
        GRRLIB_PrintfTTF(100, 100, (*state).basicFont, (*state).message, 8, 0xFFFFFFFF);
        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if((pressed & WPAD_BUTTON_A) && (pressed & WPAD_BUTTON_B)) {
            break;
        }
        GRRLIB_Render();
    }
    return (GameModeExit) { .screen = SCREEN_EXIT };
}