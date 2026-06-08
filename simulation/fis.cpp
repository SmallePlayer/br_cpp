#include "raylib.h"
#include <cmath>

int main() { 
    // размер экрана
    const int screenWidth = 800;
    const int screenHeight = 800;

    float x = 50, y = 50;

    InitWindow(screenWidth, screenHeight, "Заголовок"); 
    SetTargetFPS(60);

    float c_form = 1.0; 
    float p_rho = 1.2;
    float width = 50; 
    float area = width;
    

    float vy = 0;
    float g = 800;

    while (!WindowShouldClose()) {
        ClearBackground(RAYWHITE);
        float dt = GetFrameTime();
        vy += g * dt;
        y += vy * dt;
        
        if (y > 700){
            y = 700;
            vy = 0;
        }


        BeginDrawing();
            DrawRectangle(x, y, 50, 50, RED);
            DrawRectangle(0, 750, 800, 50, WHITE);
        EndDrawing();

    }

    CloseWindow(); // закрывает приложение
    return 0;
}