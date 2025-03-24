#include "gamemanager.h"
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include "title_png.h"
#include <stdlib.h>
#include <stdio.h>

GameModeExit splash_screen(GameState* state) {
    GRRLIB_texImg *titleImg = GRRLIB_LoadTexture(title_png);
    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    int main_alpha = 0;

    int next_screen = SCREEN_EXIT;

    int cheat_counter = 0;

    while(true) {

        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);

        bool UP_PRESSED = pressed & WPAD_BUTTON_UP;
        bool DOWN_PRESSED = pressed & WPAD_BUTTON_DOWN;
        bool LEFT_PRESSED = pressed & WPAD_BUTTON_LEFT;
        bool RIGHT_PRESSED = pressed & WPAD_BUTTON_RIGHT;
        bool A_PRESSED = pressed & WPAD_BUTTON_A;
        bool B_PRESSED = pressed & WPAD_BUTTON_B;

        if (
            (cheat_counter == 0 && UP_PRESSED) ||
            (cheat_counter == 2 && UP_PRESSED) ||
            (cheat_counter == 4 && DOWN_PRESSED) ||
            (cheat_counter == 6 && DOWN_PRESSED) ||
            (cheat_counter == 8 && LEFT_PRESSED) ||
            (cheat_counter == 10 && RIGHT_PRESSED) ||
            (cheat_counter == 12 && LEFT_PRESSED) ||
            (cheat_counter == 14 && RIGHT_PRESSED) ||
            (cheat_counter == 16 && B_PRESSED) ||
            (cheat_counter == 18 && A_PRESSED)
        ) {
            cheat_counter++;
        }
        else if(pressed == 0 && (cheat_counter & 1) == 1) {
            cheat_counter++;
        }
        else if(pressed > 0) {
            cheat_counter = 0;
        }

        if(cheat_counter == 18) {
            (*state).cheatsEnabled = true;
        }

        if(pressed & WPAD_BUTTON_HOME) {
            break;
        }
        else if (pressed & WPAD_BUTTON_2) {
            next_screen = SCREEN_CREDITS;
            break;
        }

        if(main_alpha < 255) {
            main_alpha++;
        }

        GRRLIB_DrawImg(0, 0, titleImg, 0, 1, 1, (0xFFFFFF << 8) | main_alpha);

        if((*state).cheatsEnabled)
            GRRLIB_PrintfTTF(193, 200, (*state).basicFont, "CHEATS ENABLED", 32, 0xFFFF00FF);

        GRRLIB_Render();
    }

    for(int fade = 255; fade > 0; fade-=4) {
        GRRLIB_DrawImg(0, 0, titleImg, 0, 1, 1, (0xFFFFFF << 8) | fade);
        if((*state).cheatsEnabled)
            GRRLIB_PrintfTTF(193, 200, (*state).basicFont, "CHEATS ENABLED", 32, (0xFFFF00 << 8) | fade);
        GRRLIB_Render();
    }

    GRRLIB_FreeTexture(titleImg);
    return (GameModeExit) { .screen = next_screen };
}