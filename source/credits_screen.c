#include "gamemanager.h"
#include <grrlib.h>
#include <wiiuse/wpad.h>

#include <stdio.h>

#include "credits1_png.h"
#include "credits_bg1_png.h"

GameModeExit credits_screen(GameState* state) {
    GRRLIB_texImg *creditsBg1 = GRRLIB_LoadTexture(credits_bg1_png);
    GRRLIB_texImg *credits1 = GRRLIB_LoadTexture(credits1_png);

    if(creditsBg1 == NULL) {
        strcpy((*state).message[0], "Failed to load credits background 1");
        return (GameModeExit) { .screen = SCREEN_ERROR };
    }

    if(credits1 == NULL) {
        strcpy((*state).message[0], "Failed to load credits image 1");
        return (GameModeExit) { .screen = SCREEN_ERROR };
    }

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);


    int y = 0;

    int fade = 255;

    while (true)
    {
        GRRLIB_DrawImg(0, y / 2, creditsBg1, 0, 1, 1, RGBA(255, 255, 255, 255));
        GRRLIB_DrawImg(0, y, credits1, 0, 1, 1, RGBA(255, 255, 255, fade));

        GRRLIB_SetBlend(0);

        y -= 1;

        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if(pressed & WPAD_BUTTON_HOME) {
            break;
        }

        if (y < -(credits1->h))
        {
            fade -= 1;
            if (fade < 15)
            {
                break;
            }
        }

        GRRLIB_Render();
    }
    GRRLIB_FreeTexture(creditsBg1);
    return (GameModeExit) { .screen = SCREEN_SPLASH };
}