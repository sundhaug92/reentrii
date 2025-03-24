#include <grrlib.h>
 
#include <stdlib.h>
#include <stdio.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>

#include "FreeMonoBold_ttf.h"

#include "gamemanager.h"

int main(int argc, char **argv) {
    
    GRRLIB_Init();
    WPAD_Init();
    srand(time(NULL));

    GameState state = {
        .basicFont = GRRLIB_LoadTTF(FreeMonoBold_ttf, FreeMonoBold_ttf_size),
        .frameCount = 0,
        .cheatsEnabled = false
    };

    for(int line=0; line<40; line++) {
        memset(state.message[line], 0, sizeof(state.message[line]));
    }

    GameModeExit currentMode = (GameModeExit) { .screen = SCREEN_SPLASH };

    // snprintf(state.message[0], sizeof(state.message[0]), "viTVMode=%d, fbWidth=%d, efbHeight=%d, xfbHeight=%d", rmode->viTVMode, rmode->fbWidth, rmode->efbHeight, rmode->xfbHeight);
    // snprintf(state.message[1], sizeof(state.message[1]), "viXOrigin=%d, viYOrigin=%d, viWidth=%d, viHeight=%d", rmode->viXOrigin, rmode->viYOrigin, rmode->viWidth, rmode->viHeight);
    // 
    // currentMode.screen = SCREEN_ERROR;

    while(currentMode.screen != SCREEN_EXIT) {
        switch(currentMode.screen) {
            case SCREEN_SPLASH:
                currentMode = splash_screen(&state);
                break;
            case SCREEN_CREDITS:
                currentMode = credits_screen(&state);
                break;
            default:
                snprintf(state.message[0], sizeof(state.message[0]), "Unknown screen %d", currentMode.screen);
            case SCREEN_ERROR:
                currentMode = error_screen(&state);
                break;
            }
    }

    GRRLIB_Exit();
    WPAD_Shutdown();
    exit(0);
}