#include "raylib.h"
#include <cmath>

float speed_left = 0, speed_right = 0;

void speed_state(){
    //написать прием данных 
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 800;

    // === РЕАЛЬНЫЕ ПАРАМЕТРЫ РОБОТА (в см) ===
    const float wheel_diameter = 24.0f;       // 240 мм
    const float wheel_radius = wheel_diameter / 2.0f;  // 12 см
    const float track_width = 100.0f;       // колёсная база 1000 мм = 100 см
    const float max_rpm = 1000.0f;       // 500 об/мин

    // расчёт максимальной линейной скорости (см/сек)
    // длина окружности колеса = 2 * pi * R
    // оборотов в секунду = max_rpm / 60
    const float max_speed = (2 * PI * wheel_radius) * (max_rpm / 60.0f);  // ≈ 628 см/сек
    const float robot_size = 100.0f;
    // === НАЧАЛЬНЫЕ УСЛОВИЯ ===
    float x_self = 100 + robot_size/2, y_self = 100 + robot_size/2, angle = 0;
    
    float accel = max_speed * 1;  // ускорение: разгон до макс скорости за ~0.3 сек

    

    InitWindow(screenWidth, screenHeight, "Tank");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // === УПРАВЛЕНИЕ ===
        if (IsKeyDown(KEY_E)) {
            speed_left  -= accel * dt;
            speed_right += accel * dt;
        }
        if (IsKeyDown(KEY_Q)) {
            speed_left  += accel * dt;
            speed_right -= accel * dt;
        }
        if (IsKeyDown(KEY_A)) {
            speed_left  -= accel * dt;
        }
        if (IsKeyDown(KEY_D)) {
            speed_right -= accel * dt;
        }

        // === ТРЕНИЕ ===
        speed_left  *= 0.97f;
        speed_right *= 0.97f;
        if (fabs(speed_left)  < 1) speed_left  = 0;
        if (fabs(speed_right) < 1) speed_right = 0;

        // === ЛИМИТЫ ===
        speed_left  = fmin(fmax(speed_left,  -max_speed), max_speed);
        speed_right = fmin(fmax(speed_right, -max_speed), max_speed);

        // === ДВИЖЕНИЕ ===
        float linear_speed  = (speed_left + speed_right) / 2.0f;
        float angular_speed = (speed_right - speed_left) / track_width;

        angle += angular_speed * dt;
        x_self += sin(angle) * linear_speed * dt;
        y_self -= cos(angle) * linear_speed * dt;

        if (x_self < 100) x_self = 100;
        if (x_self > screenWidth)  x_self = screenWidth;
        if (y_self < 100) y_self = 100;
        if (y_self > screenHeight)  y_self = screenHeight;

        // === ОТРИСОВКА ===
        BeginDrawing();
            ClearBackground(RAYWHITE);
            Rectangle rec = { x_self - robot_size/2, y_self - robot_size/2, robot_size, robot_size };
            Vector2 origin = { robot_size / 2.0f, robot_size / 2.0f };
            DrawRectanglePro(rec, origin, angle * RAD2DEG, RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
