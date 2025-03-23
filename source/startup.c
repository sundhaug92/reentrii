#include <grrlib.h>
 
#include <stdlib.h>
#include <stdio.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>

#include "FreeMonoBold_ttf.h"

int main(int argc, char **argv) {
    GRRLIB_Init();
    WPAD_Init();

    GRRLIB_ttfFont *myFont = GRRLIB_LoadTTF(FreeMonoBold_ttf, FreeMonoBold_ttf_size);
    GRRLIB_texImg *CopiedImg = GRRLIB_CreateEmptyTexture(rmode->fbWidth, rmode->efbHeight);
    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    srand(time(NULL));

    int frameCount = 0;
    while(true) {
        GRRLIB_DrawImg(0, 0, CopiedImg, 0, 1, 1, 0xFFFFFFFF);
        GRRLIB_PrintfTTF(100, 100, myFont, "Reentrii", 32, 0xFFFFFFFF);
        // GRRLIB_Screen2Texture(0,0, CopiedImg, false);

        for (int x = 0; x < rmode->fbWidth; x++) {
                for (int y = 0; y < rmode->efbHeight; y++) {
                    GRRLIB_SetPixelTotexImg(x, y, CopiedImg, (rand() % 0xFFFFFF) << 8 | 0xFF);
                }
            }

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