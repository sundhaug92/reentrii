#include "gamemanager.h"
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include "title_png.h"

GameModeExit splash_screen(GameState* state) {
    GRRLIB_texImg *titleImg = GRRLIB_LoadTexture(title_png);
    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);
    while(true) {
        GRRLIB_DrawImg(0, 0, titleImg, 0, 1, 1, 0xFFFFFFFF);

        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if(pressed & WPAD_BUTTON_HOME) {
            break;
        }

        GRRLIB_Render();
    }
    GRRLIB_FreeTexture(titleImg);
    return (GameModeExit) { .screen = SCREEN_EXIT };
}