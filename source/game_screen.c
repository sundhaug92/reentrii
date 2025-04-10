#include "gamemanager.h"
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "orion_png.h"
#include "hst1_png.h"
#include "hst2_png.h"
#include "hst3_png.h"
#include "bullet_friendly_png.h"
#include "bullet_hostile_png.h"
#include "f16_png.h"
#include "f15_png.h"
#include "f22_png.h"

#define clamp(x, min_value, max_value) ((x) < min_value ? min_value : ((x) > max_value ? max_value : (x)))

#define MAX_ENEMIES 10
#define MAX_ENEMY_PROJECTILES 10
#define MAX_PLAYER_PROJECTILES 50
#define MAX_BULLETS clamp(1000, MAX_PLAYER_PROJECTILES + MAX_ENEMY_PROJECTILES, 1000)
#define PLAYER_SPEED 4
#define MIN_Y (50)
#define MAX_Y (480 - 50)
#define PLAYER_X 32

#define MAX_ENEMY_SPEED 1
#define MIN_ENEMY_SPEED 0

typedef struct Projectile
{
    int x;
    int y;
    int dx;
    int dy;
    bool active;
    bool friendly;
    int damage;
} Projectile;

Projectile projectiles[MAX_BULLETS];

typedef struct Enemy
{
    int x;
    int y;
    int dx;
    int dy;
    int framesToNextMove;
    bool active;
    int health;
    int enemyType;
    int framesToNextFire;
} Enemy;

Enemy enemies[MAX_ENEMIES];

int playerY;

//                         1    2    3     4     5     6     7     8,    9,    10
int level_thresholds[] = {100, 200, 500, 1000, 1500, 2000, 3000, 5000, 7500, 10000};

void updateLevel(GameState *global_state, int newScore)
{
    #ifdef DEBUG
    printf("Updating score from %d to %d\n", (*global_state).score, newScore);
    #endif

    for (int i = 0; i < sizeof(level_thresholds) / sizeof(level_thresholds[0]); i++)
    {
        if ((*global_state).score < level_thresholds[i] && newScore >= level_thresholds[i])
        {
            (*global_state).level++;
        }
    }

    (*global_state).score = newScore;
}

int getProjectileCountOfFriendly(bool friendly)
{
    int count = 0;
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (projectiles[i].active && projectiles[i].friendly == friendly)
        {
            count++;
        }
    }
    return count;
}

bool fireProjectile(int x, int y, int dx, int dy, bool friendly, int damage)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!projectiles[i].active)
        {
            #ifdef DEBUG
            printf("Firing projectile %02d at (%d, %d) with dx %d, dy %d, friendly %d, damage %d\n", i, x, y, dx, dy, friendly, damage);
            #endif
            projectiles[i] = (Projectile){.x = x, .y = y, .dx = dx, .dy = dy, .active = true, .friendly = friendly, .damage = damage};
            return true;
        }
    }
    return false;
}

void drawAndDeactivateProjectiles(GameState *global_state, GRRLIB_texImg *enemyProjectileTexture, GRRLIB_texImg *friendlyProjectileTexture)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (projectiles[i].active)
        {
            projectiles[i].x += projectiles[i].dx;
            projectiles[i].y += projectiles[i].dy;

            // printf("Projectile %02d at %d, %d, friendly %d\n", i, projectiles[i].x, projectiles[i].y, projectiles[i].friendly);

            for (int j = 0; j < MAX_BULLETS; j++)
                if (projectiles[j].active && projectiles[j].friendly != projectiles[i].friendly)
                {
                    if (abs(projectiles[j].x - projectiles[i].x) < 32 && abs(projectiles[j].y - projectiles[i].y) < 32)
                    {
                        #ifdef DEBUG
                        printf("Projectile %02d hit projectile %02d\n", i, j);
                        #endif
                        projectiles[i].active = false;
                        projectiles[j].active = false;

                        updateLevel(global_state, (*global_state).score + 1);

                        break;
                    }
                }

            if (projectiles[i].friendly)
                for (int j = 0; j < MAX_ENEMIES; j++)
                {
                    if (enemies[j].active)
                    {
                        if (abs(enemies[j].x - projectiles[i].x) < 32 && abs(enemies[j].y - projectiles[i].y) < 32)
                        {
                            #ifdef DEBUG
                            printf("Projectile %02d hit enemy %02d\n", i, j);
                            #endif
                            projectiles[i].active = false;
                            enemies[j].health--;
                            if (enemies[j].health <= 0)
                            {
                                #ifdef DEBUG
                                printf("Deactivating enemy %02d\n", j);
                                #endif
                                enemies[j].active = false;

                                int scoreChange = 10;
                                if (enemies[j].enemyType == 1)
                                    scoreChange = 20;
                                else if (enemies[j].enemyType == 2)
                                    scoreChange = 50;
                                updateLevel(global_state, (*global_state).score + scoreChange);

                                break;
                            }
                        }
                    }
                }
            else // enemy projectile
            {
                if (pow(playerY - projectiles[i].y, 2) + pow(PLAYER_X - projectiles[i].x, 2) < 32 * 32)
                {
                    #ifdef DEBUG
                    printf("Projectile %02d hit player\n", i);
                    #endif
                    projectiles[i].active = false;
                    (*global_state).lives -= projectiles[i].damage;
                }
            }

            if (projectiles[i].x < 0 || projectiles[i].x > 640 || projectiles[i].y < 0 || projectiles[i].y > 480)
            {
                #ifdef DEBUG
                printf("Deactivating projectile %02d\n", i);
                #endif
                projectiles[i].active = false;
            }
            else
            {
                GRRLIB_DrawImg(projectiles[i].x, projectiles[i].y, projectiles[i].friendly ? friendlyProjectileTexture : enemyProjectileTexture, 0, 1, 1, 0xFFFFFFFF);
            }
        }
        else
        {
            // printf("Projectile %02d inactive\n", i);
        }
    }
}

int firstInactiveEnemy()
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].active)
        {
            return i;
        }
    }
    return -1;
}

void drawAndDeactivateEnemies(GameState *global_state, GRRLIB_texImg *enemy1Texture, GRRLIB_texImg *enemy2Texture, GRRLIB_texImg *enemy3Texture)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].active)
        {
            if (enemies[i].framesToNextMove-- <= 0)
            {
                enemies[i].x += enemies[i].dx;
                enemies[i].y += enemies[i].dy;
                enemies[i].framesToNextMove = 10;
            }

            if (enemies[i].framesToNextFire-- <= 0 && enemies[i].enemyType > 0)
            {
                enemies[i].framesToNextFire = 60;
                fireProjectile(enemies[i].x, enemies[i].y, -4, 0, false, 1);
            }

            if (enemies[i].x < 0)
            {
                #ifdef DEBUG
                printf("Deactivating enemy %02d\n", i);
                #endif
                enemies[i].active = false;
                (*global_state).lives -= 5 * enemies[i].health;
            }
            else
            {
                GRRLIB_texImg *enemyTexture = enemy1Texture;
                if (enemies[i].enemyType == 1)
                    enemyTexture = enemy2Texture;
                else if (enemies[i].enemyType == 2)
                    enemyTexture = enemy3Texture;
                GRRLIB_DrawImg(enemies[i].x, enemies[i].y, enemyTexture, 0, 1, 1, 0xFFFFFFFF);
            }
        }
        else
        {
            enemies[i].framesToNextMove = 10;
            // printf("Enemy %02d inactive\n", i);
        }
    }
}

GameModeExit game_screen(GameState *global_state)
{
    GameModeExit next_screen = {.screen = SCREEN_SPLASH};

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    // TODO: Move this somewhere else
    (*global_state).lives = (*global_state).cheatsEnabled ? 999 : 3;
    (*global_state).score = 0;
    (*global_state).level = 1;

    playerY = 480 / 2;

    char status[80];

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    GRRLIB_texImg *playerTexture = GRRLIB_LoadTexture(orion_png);

    GRRLIB_texImg *friendlyProjectileTexture = GRRLIB_LoadTexture(bullet_friendly_png);

    GRRLIB_texImg *enemy1Texture = GRRLIB_LoadTexture(f16_png);
    GRRLIB_texImg *enemy2Texture = GRRLIB_LoadTexture(f15_png);
    GRRLIB_texImg *enemy3Texture = GRRLIB_LoadTexture(f22_png);

    GRRLIB_texImg *enemyProjectileTexture = GRRLIB_LoadTexture(bullet_hostile_png);

    GRRLIB_texImg *hst1 = GRRLIB_LoadTexture(hst1_png);
    GRRLIB_texImg *hst2 = GRRLIB_LoadTexture(hst2_png);
    GRRLIB_texImg *hst3 = GRRLIB_LoadTexture(hst3_png);

    int framesSinceLastFriendlyProjectile = 0;

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        projectiles[i] = (Projectile){.active = false};
    }

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        enemies[i] = (Enemy){.active = false};
    }

    int enemySpawnRate = 60;
    int framesSinceLastEnemySpawn = 0;

    while (!(*global_state).exitRequested)
    {
        if ((*global_state).score >= 10000)
        {
            return (GameModeExit){.screen = SCREEN_VICTORY};
        }

        if ((*global_state).level > 4)
            enemySpawnRate = 10;
        else if ((*global_state).level > 2)
            enemySpawnRate = 30;

        if((*global_state).level >= 6)
            GRRLIB_DrawImg(0, 0, hst3, 0, 1, 1, 0xFFFFFFFF);
        else if((*global_state).level > 3)
            GRRLIB_DrawImg(0, 0, hst2, 0, 1, 1, 0xFFFFFFFF);
        else
            GRRLIB_DrawImg(0, 0, hst1, 0, 1, 1, 0xFFFFFFFF);
        WPAD_ScanPads();
        u32 held = WPAD_ButtonsHeld(0);

        if (held & WPAD_BUTTON_HOME)
        {
            break;
        }

        if (held & WPAD_BUTTON_UP)
        {
            playerY -= PLAYER_SPEED;
        }
        else if (held & WPAD_BUTTON_DOWN)
        {
            playerY += PLAYER_SPEED;
        }

        playerY = clamp(playerY, MIN_Y, MAX_Y);

        if ((held & WPAD_BUTTON_B) && (framesSinceLastFriendlyProjectile > 10) && getProjectileCountOfFriendly(true) < MAX_PLAYER_PROJECTILES)
        {
            framesSinceLastFriendlyProjectile = 0;

            int shots2Fire = 1;

            // TODO: Adjust difficulty
            if ((*global_state).level > 4)
                shots2Fire = 3;
            if ((*global_state).level > 6)
                shots2Fire = 5;

            while (shots2Fire > 0)
            {
                int dy = 0;
                if (shots2Fire > 3)
                    dy = (rand() % 8) - 4;
                else if (shots2Fire == 3)
                    dy = 4;
                else if (shots2Fire == 2)
                    dy = -4;

                int dx = 8;

                while ((pow(dx, 2) + pow(dy, 2) > 64) && dx > 1)
                {
                    dx--;
                }

                #ifdef DEBUG
                printf("Firing friendly projectile of type %d\n", shots2Fire);
                #endif
                if (!fireProjectile(32, playerY + (rand() % 16) - 8, dx, dy, true, 1))
                {
                    #ifdef DEBUG
                    printf("Failed to fire projectile\n");
                    #endif
                }
                shots2Fire--;
            }
        }

        if (framesSinceLastEnemySpawn > enemySpawnRate)
        {
            framesSinceLastEnemySpawn = 0;

            int availableSlot = firstInactiveEnemy();

            if (availableSlot != -1)
            {
                #ifdef DEBUG
                printf("Attempt to spawn enemy %02d\n", availableSlot);
                #endif

                bool mustChangePos = false;

                int enemyY = -1, enemyX = 640 - 32, reposAttempts = 0;
                do
                {
                    enemyY = rand() % 480;
                    enemyY = clamp(enemyY, MIN_Y, MAX_Y);

                    for (int i = 0; i < MAX_ENEMIES; i++)
                    {
                        if (enemies[i].active && ((pow(enemies[i].x - enemyX, 2) + pow(enemies[i].y - enemyY, 2)) < 32 * 32))
                        {
                            mustChangePos = true;
                            break;
                        }
                    }
                } while (mustChangePos && reposAttempts++ < 10);

                if (reposAttempts < 10)
                {
                    int maxEnemyType = 1;
                    if ((*global_state).level > 1)
                        maxEnemyType = 2;
                    if ((*global_state).level > 5)
                        maxEnemyType = 3;

                    int enemyType = -1;
                    int enemyHealth = 1;

                    int enemyTypeRoll = rand() % 100;
                    if (enemyTypeRoll < 90)
                        enemyType = 0;
                    else if (enemyTypeRoll < 95 && maxEnemyType > 1)
                    {
                        enemyType = 1;
                    }
                    else if (enemyTypeRoll < 100 && maxEnemyType > 2)
                    {
                        enemyType = 2;
                    }

                    if (enemyType == 1)
                        enemyHealth = 10;
                    else if (enemyType == 2)
                        enemyHealth = 25;

                    enemies[availableSlot] = (Enemy){.x = enemyX, .y = enemyY, .dx = -clamp((rand() % 10), MIN_ENEMY_SPEED, MAX_ENEMY_SPEED * clamp((*global_state).level, 1, 5)), .dy = 0, .active = true, .health = enemyHealth, .enemyType = enemyType, .framesToNextMove = 10};
                    #ifdef DEBUG
                    printf("Enemy %02d at %d, %d\n", availableSlot, enemies[availableSlot].x, enemies[availableSlot].y);
                    #endif
                }
                else
                {
                    #ifdef DEBUG
                    printf("Failed to spawn enemy %02d\n", availableSlot);
                    #endif
                }
            }
        }

        // printf("Player at %d, %d\n", 16, playerY);

        if ((*global_state).lives <= 0)
        {
            return (GameModeExit){.screen = SCREEN_GAME_OVER};
        }

        drawAndDeactivateEnemies(global_state, enemy1Texture, enemy2Texture, enemy3Texture);
        drawAndDeactivateProjectiles(global_state, enemyProjectileTexture, friendlyProjectileTexture);

        snprintf(status, sizeof(status), "Lives: %03d Score: %04d Level: %02d", (*global_state).lives, (*global_state).score, (*global_state).level);
        // printf("%s\n", status);
        GRRLIB_PrintfTTF(0, 0, (*global_state).basicFont, status, 24, 0xFFFFFFFF);

        GRRLIB_DrawImg(PLAYER_X, playerY, playerTexture, 90, (32.0f/216.0f), (32.0f/384.0f), 0xFFFFFFFF);
        GRRLIB_Render();
        framesSinceLastFriendlyProjectile++;
        framesSinceLastEnemySpawn++;
    }

    GRRLIB_FreeTexture(playerTexture);
    GRRLIB_FreeTexture(friendlyProjectileTexture);
    GRRLIB_FreeTexture(enemy1Texture);
    GRRLIB_FreeTexture(enemy2Texture);
    GRRLIB_FreeTexture(enemy3Texture);
    GRRLIB_FreeTexture(enemyProjectileTexture);
    return next_screen;
}