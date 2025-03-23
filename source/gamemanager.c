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

    GameState state;
    state.basicFont = GRRLIB_LoadTTF(FreeMonoBold_ttf, FreeMonoBold_ttf_size);
    state.frameCount = 0;
    
    GameModeExit currentMode = (GameModeExit) { .screen = SCREEN_SPLASH };
    
    while(currentMode.screen != SCREEN_EXIT) {
        switch(currentMode.screen) {
            case SCREEN_SPLASH:
                currentMode = splash_screen(&state);
                break;
            case SCREEN_CREDITS:
                // currentScreen = credits_screen();
                break;
            default:
                snprintf(state.message, sizeof(state.message), "Unknown screen %d", currentMode.screen);
                currentMode = error_screen(&state);
        }
    }

    GRRLIB_Exit();
    WPAD_Shutdown();
    exit(0);
}