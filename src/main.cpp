#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Test, Test");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
            //coming soon

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Hello World", 190, 200, 20, BLACK);

        EndDrawing();
    }

    CloseWindow(); 

    return 0;
}
