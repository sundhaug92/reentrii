#include <grrlib.h>
 
#include <stdlib.h>
#include <stdio.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>

#include "FreeMonoBold_ttf.h"
#include "title_png.h"

int main(int argc, char **argv) {
    GRRLIB_Init();
    WPAD_Init();

    GRRLIB_ttfFont *myFont = GRRLIB_LoadTTF(FreeMonoBold_ttf, FreeMonoBold_ttf_size);
    GRRLIB_texImg *titleImg = GRRLIB_LoadTexture(title_png);
    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    srand(time(NULL));

    int frameCount = 0;
    while(true) {
        GRRLIB_DrawImg(0, 0, titleImg, 0, 1, 1, 0xFFFFFFFF);

        WPAD_ScanPads();
        u32 pressed = WPAD_ButtonsDown(0);
        if(pressed & WPAD_BUTTON_HOME) {
            break;
        }

        GRRLIB_Render();

        frameCount++;
    }

    GRRLIB_Exit();
    WPAD_Shutdown();
    exit(0);
}