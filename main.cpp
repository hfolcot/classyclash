#include "raylib.h"
#include "raymath.h"
#include "character.h"
#include "prop.h"
#include "enemy.h"
#include <string>

int main()
{
    int windowDimensions[2]{GetMonitorWidth(2), GetMonitorHeight(2)};
    InitWindow(windowDimensions[0], windowDimensions[1], "Top Down Adventure");
    MaximizeWindow();

    Texture2D gameMap = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{8.0f};

    Character knight(windowDimensions);

    Prop props[2]{
        Prop{Vector2{1200.f, 600.f}, LoadTexture("nature_tileset/Rock.png")},

        Prop{Vector2{800.f, 1000.f}, LoadTexture("nature_tileset/Log.png")},
    };

    Enemy goblin(Vector2{400.f, 400.f}, LoadTexture("characters/goblin_idle_spritesheet.png"), LoadTexture("characters/goblin_run_spritesheet.png"));
    Enemy slime(Vector2{250.f, 500.f}, LoadTexture("characters/slime_idle_spritesheet.png"), LoadTexture("characters/slime_run_spritesheet.png"));
    // goblin.setTarget(&knight);

    Enemy *enemies[]{
        &goblin,
        &slime};

    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // Draw Map
        DrawTextureEx(gameMap, mapPos, 0.0, mapScale, WHITE);

        for (auto prop : props)
        {
            prop.render(knight.getWorldPos());
        }

        //Display relevant text
        if (!knight.getAlive())
        {
            // Game lost
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            // Game continues
            std::string knightsHealth = "Health: ";
            knightsHealth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightsHealth.c_str(), 55.f, 45.f, 40, RED);
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                }
            }
        }

        knight.tick(GetFrameTime());
        // Check map bounds
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowDimensions[0] > gameMap.width * mapScale ||
            knight.getWorldPos().y + windowDimensions[1] > gameMap.height * mapScale)
        {
            knight.undoMovement();
        }

        for (auto prop : props)
        {
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                knight.undoMovement();
            }
        }
        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }

        EndDrawing();
    }
    UnloadTexture(gameMap);
    CloseWindow();
}