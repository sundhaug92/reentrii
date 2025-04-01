#include <grrlib.h>

#include <stdlib.h>
#include <stdio.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>

#include "FreeMonoBold_ttf.h"

#include "gamemanager.h"

int main(int argc, char **argv)
{

    GRRLIB_Init();
    WPAD_Init();
    srand(time(NULL));

    GameState global_state = {
        .basicFont = GRRLIB_LoadTTF(FreeMonoBold_ttf, FreeMonoBold_ttf_size),
        .frameCount = 0,
        .cheatsEnabled = false};

    for (int line = 0; line < 40; line++)
    {
        memset(global_state.message[line], 0, sizeof(global_state.message[line]));
    }

    GameModeExit currentMode = (GameModeExit){.screen = SCREEN_SPLASH};

    SYS_STDIO_Report(true);

    while (currentMode.screen != SCREEN_EXIT)
    {
        switch (currentMode.screen)
        {
        case SCREEN_SPLASH:
            currentMode = splash_screen(&global_state);
            break;
        case SCREEN_CREDITS:
            currentMode = credits_screen(&global_state);
            break;
        case SCREEN_GAME:
            currentMode = game_screen(&global_state);
            break;
        // TODO: Implement these screens
        case SCREEN_GAME_OVER:
            currentMode = gameover_screen(&global_state);
            break;
        case SCREEN_VICTORY:
            currentMode = victory_screen(&global_state);
            break;
        case SCREEN_STORY:
            currentMode = story_screen(&global_state);
            break;
        default:
            snprintf(global_state.message[0], sizeof(global_state.message[0]), "Unknown screen %d", currentMode.screen);
            // Fall through here and display the error screen
        case SCREEN_ERROR:
            currentMode = error_screen(&global_state);
            break;
        }
        
        GX_WaitDrawDone();
    }

    GRRLIB_Exit();
    WPAD_Shutdown();
    exit(0);
}