#include "gamemanager.h"
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include "title_png.h"
#include <stdlib.h>
#include <stdio.h>
#include "orion_png.h"

#define PROMPT_ALPHA_MIN 64
#define PROMPT_ALPHA_MAX 255
#define PROMPT_ALPHA_SPEED 1

#define ORION_X 140
#define ORION_Y 220
#define ORION_SCALE .5f

GameModeExit splash_screen(GameState *global_state)
{
    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);
    GRRLIB_texImg *titleImg = GRRLIB_LoadTexture(title_png);

    if (titleImg == NULL)
    {
        strcpy((*global_state).message[0], "Failed to load title image");
        return (GameModeExit){.screen = SCREEN_ERROR};
    }

    GRRLIB_texImg *orionImg = GRRLIB_LoadTexture(orion_png);
    if (orionImg == NULL)
    {
        strcpy((*global_state).message[0], "Failed to load orion image");
        return (GameModeExit){.screen = SCREEN_ERROR};
    }

    // TODO: Consider using an actual texture for the prompt
    GRRLIB_texImg *promptImg = GRRLIB_CreateEmptyTexture(640, 32);
    GRRLIB_CompoStart();
    GRRLIB_PrintfTTF(0, 0, (*global_state).basicFont, "Press 1 for game, press 2 for credits", 24, 0xFFFFFFFF);
    GRRLIB_CompoEnd(0, 0, promptImg);

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    int main_alpha = 0;
    int prompt_alpha = 0;
    int prompt_alpha_delta = 0;

    int next_screen = SCREEN_EXIT;

    int cheat_counter = 0;

    while (!(*global_state).exitRequested)
    {

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
            (cheat_counter == 18 && A_PRESSED))
        {
            cheat_counter++;
            #ifdef DEBUG
            printf("Cheat counter: %d\n", cheat_counter);
            #endif
        }
        else if (pressed == 0 && (cheat_counter & 1) == 1)
        {
            cheat_counter++;
            
            #ifdef DEBUG
            printf("Cheat counter: %d\n", cheat_counter);
            #endif
        }
        else if (pressed > 0)
        {
            
            #ifdef DEBUG
            printf("Cheat counter reset\n");
            #endif
            cheat_counter = 0;
        }

        if (cheat_counter == 20)
        {
            #ifdef DEBUG
            printf("Enabling cheats\n");
            #endif
            (*global_state).cheatsEnabled = true;
            cheat_counter = 0; // Prevent spamming the log
        }

        if (pressed & WPAD_BUTTON_HOME)
        {
            break;
        }
        else if (pressed & WPAD_BUTTON_1)
        {
            next_screen = SCREEN_STORY;
            break;
        }
        else if (pressed & WPAD_BUTTON_2)
        {
            next_screen = SCREEN_CREDITS;
            break;
        }

        if (main_alpha < 255)
        {
            main_alpha++;
        }
        else if (prompt_alpha_delta == 0)
        {
            prompt_alpha_delta = PROMPT_ALPHA_SPEED;
            prompt_alpha = PROMPT_ALPHA_MAX;
        }

        // printf("main_alpha: %d, prompt_alpha: %d, prompt_alpha_delta: %d\n", main_alpha, prompt_alpha, prompt_alpha_delta);
        prompt_alpha += prompt_alpha_delta;
        if ((prompt_alpha <= PROMPT_ALPHA_MIN && prompt_alpha_delta < 0) || (prompt_alpha >= PROMPT_ALPHA_MAX && prompt_alpha_delta > 0))
        {
            prompt_alpha_delta *= -1;
        }

        GRRLIB_DrawImg(0, 0, titleImg, 0, 1, 1, (0xFFFFFF << 8) | main_alpha);
        GRRLIB_DrawImg(ORION_X, ORION_Y, orionImg, 0, 1, 1, (0xFFFFFF << 8) | main_alpha);
        GRRLIB_DrawImg(0, 480 - 24, promptImg, 0, 1, 1, (0xFFFFFF << 8) | prompt_alpha);

        if ((*global_state).cheatsEnabled)
            GRRLIB_PrintfTTF(193, 200, (*global_state).basicFont, "CHEATS ENABLED", 32, 0xFFFF00FF);

        GRRLIB_Render();
    }

    for (int fade = 255; fade > 0; fade -= 4)
    {
        GRRLIB_DrawImg(0, 0, titleImg, 0, 1, 1, (0xFFFFFF << 8) | fade);
        GRRLIB_DrawImg(ORION_X, ORION_Y, orionImg, 0, 1, 1, (0xFFFFFF << 8) | fade);
        if ((*global_state).cheatsEnabled)
            GRRLIB_PrintfTTF(193, 200, (*global_state).basicFont, "CHEATS ENABLED", 32, (0xFFFF00 << 8) | fade);
        GRRLIB_Render();
    }

    GRRLIB_FreeTexture(titleImg);
    GRRLIB_FreeTexture(promptImg);
    GRRLIB_FreeTexture(orionImg);


    return (GameModeExit){.screen = next_screen};
}