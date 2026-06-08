#include "raylib.h"

int main() {
    const int sw = 800;
    const int sh = 600;
    InitWindow(sw, sh, "Рисование в raylib");
    SetTargetFPS(60);

    // Загружаем ресурсы
    Texture2D tex = LoadTexture("player.png"); // можно заменить на любую PNG

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // ========== 2D ФИГУРЫ ==========
        DrawLine(10, 10, 200, 10, RED);
        DrawLineEx({10, 30}, {200, 30}, 5, BLUE);
        DrawCircle(100, 120, 40, GREEN);
        DrawCircleLines(100, 120, 40, DARKGREEN);
        DrawCircleSector({100, 120}, 50, 0, 90, 36, ORANGE);
        DrawRing({250, 120}, 30, 50, 0, 360, 36, PURPLE);
        DrawEllipse(400, 120, 60, 30, PINK);

        DrawRectangle(10, 200, 100, 60, RED);
        DrawRectangleLines(10, 280, 100, 60, BLUE);
        DrawRectangleRounded({130, 200, 100, 60}, 0.3, 10, GREEN);
        DrawRectanglePro({250, 200, 100, 60}, {50, 30}, 30, ORANGE);
        DrawRectangleGradientV(370, 200, 100, 60, RED, BLUE);

        DrawTriangle({10, 350}, {60, 290}, {110, 350}, PURPLE);
        DrawTriangleLines({130, 350}, {180, 290}, {230, 350}, PURPLE);
        DrawPoly({400, 350}, 6, 50, 0, MAROON);

        // ========== СПЛАЙНЫ ==========
        Vector2 pts[] = {{10, 420}, {100, 360}, {200, 440}, {300, 380}};
        DrawSplineCatmullRom(pts, 4, 3, DARKBLUE);
        DrawCircleV(pts[0], 4, RED);
        DrawCircleV(pts[1], 4, RED);
        DrawCircleV(pts[2], 4, RED);
        DrawCircleV(pts[3], 4, RED);

        Vector2 bez[] = {{350, 420}, {400, 360}, {450, 440}, {500, 380}};
        DrawSplineBezierCubic(bez, 4, 3, ORANGE);

        // ========== ТЕКСТУРА ==========
        if (IsTextureValid(tex)) {
            DrawTexture(tex, 550, 10, WHITE);
            DrawTextureEx(tex, {550, 120}, 45, 1.5, WHITE);
            DrawTextureRec(tex, {0, 0, (float)tex.width/2, (float)tex.height},
                           {550, 250}, WHITE);
            DrawTexturePro(tex,
                {0, 0, (float)tex.width, (float)tex.height},
                {550, 370, 128, 128},
                {64, 64}, 0, WHITE);
        } else {
            DrawText("Нет player.png - нарисуем квадрат вместо текстуры",
                     550, 10, 10, GRAY);
            DrawRectangle(550, 50, 64, 64, RED);
            DrawRectangleLines(550, 50, 64, 64, BLACK);
        }

        // ========== ТЕКСТ ==========
        DrawText("Фигуры", 10, sh - 60, 20, DARKGRAY);
        DrawFPS(sw - 100, sh - 30);

        // ========== КОЛЛИЗИИ ==========
        Rectangle rectA = {10, 480, 50, 50};
        Rectangle rectB = {(float)GetMouseX(), (float)GetMouseY(), 50, 50};
        Color colA = CheckCollisionRecs(rectA, rectB) ? RED : GREEN;
        DrawRectangleRec(rectA, colA);
        DrawRectangleRec(rectB, BLUE);

        Color pixCol = GetColor(0x000000FF);
        Vector2 mouse = GetMousePosition();
        DrawCircleV(mouse, 5, BLACK);

        EndDrawing();
    }

    if (IsTextureValid(tex)) UnloadTexture(tex);
    CloseWindow();
    return 0;
}
