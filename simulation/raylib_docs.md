# Полное руководство по библиотеке raylib v5.5

---

## 1. Обзор

### Что такое raylib

raylib — это простая и удобная библиотека для программирования видеоигр на языке C (C99). Она спроектирована так, чтобы быть максимально простой в использовании, не требуя от программиста глубоких знаний графических API. Весь код библиотеки написан в стиле PascalCase/camelCase.

### Ключевые особенности

- **Нет внешних зависимостей** — все необходимые библиотеки уже включены в состав raylib
- **Кроссплатформенность** — Windows, Linux, FreeBSD, OpenBSD, NetBSD, DragonFly, macOS, Haiku, Android, Raspberry Pi, DRM native, HTML5
- **Аппаратное ускорение** через OpenGL (1.1, 2.1, 3.3, 4.3, ES2, ES3)
- **Уникальный слой абстракции OpenGL** (rlgl) — может использоваться как самостоятельный модуль
- **Поддержка множества форматов шрифтов**: TTF, OTF, FNT, BDF, Sprite fonts
- **Поддержка сжатых форматов текстур**: DXT, ETC, ASTC
- **Полноценное 3D** — фигуры, модели, билборды, карты высот
- **Гибкая система материалов** — классические и PBR карты
- **Анимированные 3D модели** (скелетная анимация): IQM, M3D, GLTF
- **Шейдеры** — модельные и постобработки
- **Мощный математический модуль**: raymath (векторы, матрицы, кватернионы)
- **Аудио** — загрузка и воспроизведение с поддержкой потоковой передачи (WAV, OGG, MP3, FLAC, QOA, XM, MOD)
- **VR стерео-рендеринг** с настраиваемыми параметрами HMD

### Версия

Текущая версия: **5.5** (major=5, minor=5, patch=0)

Заголовочный файл: `/opt/homebrew/include/raylib.h`

---

## 2. Система сборки (CMake)

### Полный разбор CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
```

Указывает минимальную версию CMake, необходимую для сборки. Если версия CMake на системе меньше 3.16 — сборка не запустится.

```cmake
project(RobotSim CXX)
```

Задаёт имя проекта (`RobotSim`) и язык — C++. Без указания `CXX` CMake может не определить компилятор C++.

```cmake
set(CMAKE_CXX_STANDARD 17)
```

Указывает стандарт C++17. Компилятор будет вызван с флагом `-std=c++17`.

```cmake
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

Делает стандарт обязательным — если компилятор не поддерживает C++17, сборка прервётся с ошибкой.

```cmake
set(CMAKE_PREFIX_PATH "/opt/homebrew")
```

Указывает CMake, где искать установленные пакеты. `/opt/homebrew` — это стандартный путь установки Homebrew на Apple Silicon (ARM) Mac. CMake будет искать в `/opt/homebrew/lib/cmake/` файлы конфигурации пакетов (например, `raylib-config.cmake`).

```cmake
find_package(raylib REQUIRED)
```

Ищет установленный пакет raylib. `REQUIRED` означает, что если пакет не найден — сборка прервётся с фатальной ошибкой. CMake ищет файл `raylib-config.cmake` или `raylib-config-version.cmake` в стандартных путях и в `CMAKE_PREFIX_PATH`. Этот файл определяет переменные:
- `raylib_INCLUDE_DIRS` — пути к заголовочным файлам
- `raylib_LIBRARIES` — пути к библиотекам
- `raylib_FOUND` — был ли найден пакет

```cmake
add_executable(robot_sim main.cpp)
```

Создаёт цель сборки — исполняемый файл `robot_sim` из исходного файла `main.cpp`.

```cmake
target_link_libraries(robot_sim raylib)
```

Линкует исполняемый файл `robot_sim` с библиотекой raylib. `raylib` здесь — это имя цели (target), импортированной `find_package`. Линковка подключает:
- Пути к заголовочным файлам raylib (через `target_include_directories`)
- Саму библиотеку (статическую или динамическую)
- Транзитивные зависимости raylib (OpenGL, GLFW и т.д.)

### Как работает линковка

1. **Компиляция**: `main.cpp` компилируется в объектный файл `main.o`. Компилятору нужны заголовочные файлы raylib (путь передаётся через `-I`).
2. **Линковка**: Линкер собирает `main.o` вместе с библиотекой raylib в исполняемый файл. На macOS raylib может быть статической (`.a`) или динамической (`.dylib`) библиотекой.
3. **Динамическая линковка**: Библиотека `.dylib` загружается при запуске программы. Должна быть доступна по стандартному пути или через `DYLD_LIBRARY_PATH`.
4. **Статическая линковка**: Код библиотеки копируется в исполняемый файл. Файл получается больше, но не требует наличия библиотеки в системе.

### Что такое CMAKE_PREFIX_PATH

Это список путей, где CMake ищет пакеты (через `find_package`), библиотеки и заголовочные файлы. На Apple Silicon Mac Homebrew устанавливает всё в `/opt/homebrew`, поэтому указание `CMAKE_PREFIX_PATH` обязательно.

### Что такое find_package

Команда CMake, которая ищет и загружает настройки внешнего пакета. Она ищет файлы:
- `Find<PackageName>.cmake` — модуль поиска
- `<PackageName>Config.cmake` — конфигурационный файл, поставляемый с пакетом

Для raylib Homebrew устанавливает `raylib-config.cmake` в `/opt/homebrew/lib/cmake/raylib/`.

### Что такое target_link_libraries

Указывает, с какими библиотеками линковать цель. В современном CMake это не просто добавление `-lraylib`, но и:
- Пропаганда требований к include directories
- Пропаганда требований к компиляции
- Подключение транзитивных зависимостей

---

## 3. Полный API Reference

### 3.1 Окно и устройство (Window and Graphics Device)

#### `InitWindow`
```c
void InitWindow(int width, int height, const char *title);
```
Инициализирует окно и OpenGL контекст. **Должна быть вызвана первой** перед любыми другими функциями raylib (кроме `SetConfigFlags`, `SetTraceLogLevel` и подобных).

- `width` — ширина окна в пикселях
- `height` — высота окна в пикселях
- `title` — заголовок окна (C-строка)

**Пример:**
```c
InitWindow(800, 600, "Моя игра");
```

#### `CloseWindow`
```c
void CloseWindow(void);
```
Закрывает окно и выгружает OpenGL контекст. Вызывается в конце программы, перед `return 0`.

#### `WindowShouldClose`
```c
bool WindowShouldClose(void);
```
Проверяет, должно ли приложение закрыться (нажат ESC или клик на крестик окна). Возвращает `true`, если окно должно закрыться.

Используется в условии главного цикла:
```c
while (!WindowShouldClose()) { ... }
```

#### `IsWindowReady`
```c
bool IsWindowReady(void);
```
Проверяет, успешно ли инициализировано окно. Возвращает `true` после `InitWindow`.

#### `IsWindowFullscreen`
```c
bool IsWindowFullscreen(void);
```
Проверяет, находится ли окно в полноэкранном режиме.

#### `IsWindowHidden`
```c
bool IsWindowHidden(void);
```
Проверяет, скрыто ли окно.

#### `IsWindowMinimized`
```c
bool IsWindowMinimized(void);
```
Проверяет, свёрнуто ли окно.

#### `IsWindowMaximized`
```c
bool IsWindowMaximized(void);
```
Проверяет, развёрнуто ли окно на весь экран.

#### `IsWindowFocused`
```c
bool IsWindowFocused(void);
```
Проверяет, находится ли окно в фокусе.

#### `IsWindowResized`
```c
bool IsWindowResized(void);
```
Проверяет, было ли изменено окно за последний кадр.

#### `IsWindowState`
```c
bool IsWindowState(unsigned int flag);
```
Проверяет, включён ли указанный флаг окна.

- `flag` — один из `ConfigFlags` (например, `FLAG_WINDOW_RESIZABLE`)

#### `SetWindowState`
```c
void SetWindowState(unsigned int flags);
```
Устанавливает флаги окна. Можно комбинировать через `|`.

- `flags` — битовая маска флагов из `ConfigFlags`

**Пример:**
```c
SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
```

#### `ClearWindowState`
```c
void ClearWindowState(unsigned int flags);
```
Снимает указанные флаги окна.

#### `ToggleFullscreen`
```c
void ToggleFullscreen(void);
```
Переключает режим окна: полноэкранный/оконный. Размер окна подстраивается под разрешение монитора.

#### `ToggleBorderlessWindowed`
```c
void ToggleBorderlessWindowed(void);
```
Переключает в безрамочный оконный режим (borderless windowed). Окно подстраивается под разрешение монитора.

#### `MaximizeWindow`
```c
void MaximizeWindow(void);
```
Разворачивает окно (если установлен флаг `FLAG_WINDOW_RESIZABLE`).

#### `MinimizeWindow`
```c
void MinimizeWindow(void);
```
Сворачивает окно.

#### `RestoreWindow`
```c
void RestoreWindow(void);
```
Восстанавливает окно из свёрнутого/развёрнутого состояния.

#### `SetWindowIcon`
```c
void SetWindowIcon(Image image);
```
Устанавливает иконку окна (одно изображение, RGBA 32bit).

#### `SetWindowIcons`
```c
void SetWindowIcons(Image *images, int count);
```
Устанавливает несколько иконок окна для разных размеров.

#### `SetWindowTitle`
```c
void SetWindowTitle(const char *title);
```
Меняет заголовок окна.

#### `SetWindowPosition`
```c
void SetWindowPosition(int x, int y);
```
Устанавливает позицию окна на экране.

#### `SetWindowMonitor`
```c
void SetWindowMonitor(int monitor);
```
Устанавливает монитор для окна.

#### `SetWindowMinSize`
```c
void SetWindowMinSize(int width, int height);
```
Устанавливает минимальный размер окна (для `FLAG_WINDOW_RESIZABLE`).

#### `SetWindowMaxSize`
```c
void SetWindowMaxSize(int width, int height);
```
Устанавливает максимальный размер окна.

#### `SetWindowSize`
```c
void SetWindowSize(int width, int height);
```
Устанавливает размер окна.

#### `SetWindowOpacity`
```c
void SetWindowOpacity(float opacity);
```
Устанавливает прозрачность окна [0.0f — полностью прозрачно, 1.0f — непрозрачно].

#### `SetWindowFocused`
```c
void SetWindowFocused(void);
```
Переводит фокус на окно.

#### `GetWindowHandle`
```c
void *GetWindowHandle(void);
```
Возвращает нативный handle окна (для платформозависимых операций).

#### `GetScreenWidth`
```c
int GetScreenWidth(void);
```
Возвращает текущую ширину экрана (окна) в пикселях.

#### `GetScreenHeight`
```c
int GetScreenHeight(void);
```
Возвращает текущую высоту экрана (окна) в пикселях.

#### `GetRenderWidth`
```c
int GetRenderWidth(void);
```
Возвращает ширину рендера (учитывает HiDPI — может отличаться от `GetScreenWidth`). На HiDPI мониторах может быть в 2 раза больше.

#### `GetRenderHeight`
```c
int GetRenderHeight(void);
```
Возвращает высоту рендера (учитывает HiDPI).

#### `GetMonitorCount`
```c
int GetMonitorCount(void);
```
Возвращает количество подключённых мониторов.

#### `GetCurrentMonitor`
```c
int GetCurrentMonitor(void);
```
Возвращает индекс монитора, на котором находится окно.

#### `GetMonitorPosition`
```c
Vector2 GetMonitorPosition(int monitor);
```
Возвращает позицию монитора на виртуальном рабочем столе.

#### `GetMonitorWidth`
```c
int GetMonitorWidth(int monitor);
```
Возвращает ширину монитора в текущем видеорежиме.

#### `GetMonitorHeight`
```c
int GetMonitorHeight(int monitor);
```
Возвращает высоту монитора в текущем видеорежиме.

#### `GetMonitorPhysicalWidth`
```c
int GetMonitorPhysicalWidth(int monitor);
```
Возвращает физическую ширину монитора в миллиметрах.

#### `GetMonitorPhysicalHeight`
```c
int GetMonitorPhysicalHeight(int monitor);
```
Возвращает физическую высоту монитора в миллиметрах.

#### `GetMonitorRefreshRate`
```c
int GetMonitorRefreshRate(int monitor);
```
Возвращает частоту обновления монитора в Гц.

#### `GetWindowPosition`
```c
Vector2 GetWindowPosition(void);
```
Возвращает позицию окна на мониторе.

#### `GetWindowScaleDPI`
```c
Vector2 GetWindowScaleDPI(void);
```
Возвращает DPI-масштаб окна (обычно (1.0, 1.0) или (2.0, 2.0) на Retina).

#### `GetMonitorName`
```c
const char *GetMonitorName(int monitor);
```
Возвращает человекочитаемое UTF-8 название монитора.

#### `SetClipboardText`
```c
void SetClipboardText(const char *text);
```
Устанавливает текст в буфер обмена.

#### `GetClipboardText`
```c
const char *GetClipboardText(void);
```
Возвращает текст из буфера обмена.

#### `GetClipboardImage`
```c
Image GetClipboardImage(void);
```
Возвращает изображение из буфера обмена.

#### `EnableEventWaiting`
```c
void EnableEventWaiting(void);
```
Включает ожидание событий в `EndDrawing()` (без автоматического опроса). Может снизить использование CPU.

#### `DisableEventWaiting`
```c
void DisableEventWaiting(void);
```
Выключает ожидание событий (автоматический опрос).

### 3.2 Курсор (Cursor)

#### `ShowCursor`
```c
void ShowCursor(void);
```
Показывает курсор мыши.

#### `HideCursor`
```c
void HideCursor(void);
```
Прячет курсор мыши.

#### `IsCursorHidden`
```c
bool IsCursorHidden(void);
```
Проверяет, скрыт ли курсор.

#### `EnableCursor`
```c
void EnableCursor(void);
```
Разблокирует курсор (разрешает выход за границы окна).

#### `DisableCursor`
```c
void DisableCursor(void);
```
Блокирует курсор (остаётся в центре окна). Используется для FPS камеры.

#### `IsCursorOnScreen`
```c
bool IsCursorOnScreen(void);
```
Проверяет, находится ли курсор в пределах окна.

### 3.3 Рисование (Drawing)

#### `ClearBackground`
```c
void ClearBackground(Color color);
```
Очищает экран указанным цветом. Должна вызываться внутри `BeginDrawing()`/`EndDrawing()`.

#### `BeginDrawing`
```c
void BeginDrawing(void);
```
Начинает рисование. Все команды рисования должны быть между `BeginDrawing()` и `EndDrawing()`.

#### `EndDrawing`
```c
void EndDrawing(void);
```
Заканчивает рисование и меняет буферы (double buffering).

#### `BeginMode2D`
```c
void BeginMode2D(Camera2D camera);
```
Начинает 2D режим с кастомной камерой.

#### `EndMode2D`
```c
void EndMode2D(void);
```
Заканчивает 2D режим с камерой.

#### `BeginMode3D`
```c
void BeginMode3D(Camera3D camera);
```
Начинает 3D режим с камерой.

#### `EndMode3D`
```c
void EndMode3D(void);
```
Заканчивает 3D режим и возвращается к 2D ортографическому режиму.

#### `BeginTextureMode`
```c
void BeginTextureMode(RenderTexture2D target);
```
Перенаправляет всё рисование в текстуру рендера (FBO).

#### `EndTextureMode`
```c
void EndTextureMode(void);
```
Заканчивает рисование в текстуру рендера.

#### `BeginShaderMode`
```c
void BeginShaderMode(Shader shader);
```
Включает кастомный шейдер для последующего рисования.

#### `EndShaderMode`
```c
void EndShaderMode(void);
```
Выключает кастомный шейдер (возвращает стандартный).

#### `BeginBlendMode`
```c
void BeginBlendMode(int mode);
```
Включает режим смешивания цветов. `mode`: `BLEND_ALPHA`, `BLEND_ADDITIVE`, `BLEND_MULTIPLIED`, `BLEND_ADD_COLORS`, `BLEND_SUBTRACT_COLORS`, `BLEND_CUSTOM`.

#### `EndBlendMode`
```c
void EndBlendMode(void);
```
Выключает кастомный режим смешивания.

#### `BeginScissorMode`
```c
void BeginScissorMode(int x, int y, int width, int height);
```
Начинает scissor-режим — всё рисование будет обрезано по прямоугольнику.

#### `EndScissorMode`
```c
void EndScissorMode(void);
```
Заканчивает scissor-режим.

#### `BeginVrStereoMode`
```c
void BeginVrStereoMode(VrStereoConfig config);
```
Начинает стерео-рендеринг для VR.

#### `EndVrStereoMode`
```c
void EndVrStereoMode(void);
```
Заканчивает стерео-рендеринг VR.

#### `LoadVrStereoConfig`
```c
VrStereoConfig LoadVrStereoConfig(VrDeviceInfo device);
```
Загружает VR стерео-конфигурацию.

#### `UnloadVrStereoConfig`
```c
void UnloadVrStereoConfig(VrStereoConfig config);
```
Выгружает VR стерео-конфигурацию.

### 3.4 Шейдеры (Shader Management)

#### `LoadShader`
```c
Shader LoadShader(const char *vsFileName, const char *fsFileName);
```
Загружает шейдер из файлов. `NULL` = стандартный.

#### `LoadShaderFromMemory`
```c
Shader LoadShaderFromMemory(const char *vsCode, const char *fsCode);
```
Загружает шейдер из строк кода.

#### `IsShaderValid`
```c
bool IsShaderValid(Shader shader);
```
Проверяет, загружен ли шейдер на GPU.

#### `GetShaderLocation`
```c
int GetShaderLocation(Shader shader, const char *uniformName);
```
Получает location uniform-переменной по имени.

#### `GetShaderLocationAttrib`
```c
int GetShaderLocationAttrib(Shader shader, const char *attribName);
```
Получает location атрибута по имени.

#### `SetShaderValue`
```c
void SetShaderValue(Shader shader, int locIndex, const void *value, int uniformType);
```
Устанавливает значение uniform-переменной.

#### `SetShaderValueV`
```c
void SetShaderValueV(Shader shader, int locIndex, const void *value, int uniformType, int count);
```
Устанавливает массив uniform-значений.

#### `SetShaderValueMatrix`
```c
void SetShaderValueMatrix(Shader shader, int locIndex, Matrix mat);
```
Устанавливает матрицу 4x4 в uniform.

#### `SetShaderValueTexture`
```c
void SetShaderValueTexture(Shader shader, int locIndex, Texture2D texture);
```
Устанавливает текстуру (sampler2d) в uniform.

#### `UnloadShader`
```c
void UnloadShader(Shader shader);
```
Выгружает шейдер из GPU памяти.

### 3.5 Экранные координаты (Screen Space)

#### `GetScreenToWorldRay`
```c
Ray GetScreenToWorldRay(Vector2 position, Camera camera);
```
Создаёт луч из позиции на экране в 3D мир.

#### `GetScreenToWorldRayEx`
```c
Ray GetScreenToWorldRayEx(Vector2 position, Camera camera, int width, int height);
```
Создаёт луч из позиции с указанием viewport.

#### `GetWorldToScreen`
```c
Vector2 GetWorldToScreen(Vector3 position, Camera camera);
```
Преобразует 3D позицию в экранные 2D координаты.

#### `GetWorldToScreenEx`
```c
Vector2 GetWorldToScreenEx(Vector3 position, Camera camera, int width, int height);
```
Преобразует 3D в экранные с параметрами viewport.

#### `GetWorldToScreen2D`
```c
Vector2 GetWorldToScreen2D(Vector2 position, Camera2D camera);
```
Преобразует мировые 2D координаты в экранные.

#### `GetScreenToWorld2D`
```c
Vector2 GetScreenToWorld2D(Vector2 position, Camera2D camera);
```
Преобразует экранные 2D координаты в мировые.

#### `GetCameraMatrix`
```c
Matrix GetCameraMatrix(Camera camera);
```
Возвращает view matrix камеры.

#### `GetCameraMatrix2D`
```c
Matrix GetCameraMatrix2D(Camera2D camera);
```
Возвращает 2D матрицу трансформации камеры.

### 3.6 Тайминг (Timing)

#### `SetTargetFPS`
```c
void SetTargetFPS(int fps);
```
Устанавливает максимальный FPS.

#### `GetFrameTime`
```c
float GetFrameTime(void);
```
Возвращает delta time последнего кадра (секунды). Используется для плавной анимации.

#### `GetTime`
```c
double GetTime(void);
```
Возвращает время (секунды) с `InitWindow`.

#### `GetFPS`
```c
int GetFPS(void);
```
Возвращает текущий FPS.

### 3.7 Custom Frame Control

#### `SwapScreenBuffer`
```c
void SwapScreenBuffer(void);
```
Меняет задний буфер с передним.

#### `PollInputEvents`
```c
void PollInputEvents(void);
```
Регистрирует все события ввода.

#### `WaitTime`
```c
void WaitTime(double seconds);
```
Приостанавливает выполнение.

### 3.8 Генерация случайных чисел

#### `SetRandomSeed`
```c
void SetRandomSeed(unsigned int seed);
```
Устанавливает зерно для ГСЧ.

#### `GetRandomValue`
```c
int GetRandomValue(int min, int max);
```
Случайное число [min, max].

#### `LoadRandomSequence`
```c
int *LoadRandomSequence(unsigned int count, int min, int max);
```
Последовательность случайных чисел без повторений.

#### `UnloadRandomSequence`
```c
void UnloadRandomSequence(int *sequence);
```
Выгружает последовательность.

### 3.9 Разное (Misc)

#### `TakeScreenshot`
```c
void TakeScreenshot(const char *fileName);
```
Делает скриншот. Расширение определяет формат (`.png`, `.bmp`, `.tga`, `.jpg`).

#### `SetConfigFlags`
```c
void SetConfigFlags(unsigned int flags);
```
Устанавливает флаги **до** `InitWindow()`.

#### `OpenURL`
```c
void OpenURL(const char *url);
```
Открывает URL в браузере.

#### `TraceLog`
```c
void TraceLog(int logLevel, const char *text, ...);
```
Выводит сообщение в лог (printf-стиль).

#### `SetTraceLogLevel`
```c
void SetTraceLogLevel(int logLevel);
```
Устанавливает пороговый уровень лога.

#### `MemAlloc`
```c
void *MemAlloc(unsigned int size);
```
Аллокатор памяти (обёртка над malloc).

#### `MemRealloc`
```c
void *MemRealloc(void *ptr, unsigned int size);
```
Реаллокатор.

#### `MemFree`
```c
void MemFree(void *ptr);
```
Освободитель памяти.

#### `SetTraceLogCallback`
```c
void SetTraceLogCallback(TraceLogCallback callback);
```
Кастомный колбэк логирования.

#### `SetLoadFileDataCallback`
```c
void SetLoadFileDataCallback(LoadFileDataCallback callback);
```
Кастомный загрузчик бинарных данных.

#### `SetSaveFileDataCallback`
```c
void SetSaveFileDataCallback(SaveFileDataCallback callback);
```
Кастомный сохранятель бинарных данных.

#### `SetLoadFileTextCallback`
```c
void SetLoadFileTextCallback(LoadFileTextCallback callback);
```
Кастомный загрузчик текста.

#### `SetSaveFileTextCallback`
```c
void SetSaveFileTextCallback(SaveFileTextCallback callback);
```
Кастомный сохранятель текста.

### 3.10 Файлы и данные (File Management)

#### `LoadFileData`
```c
unsigned char *LoadFileData(const char *fileName, int *dataSize);
```
Загружает данные из файла.

#### `UnloadFileData`
```c
void UnloadFileData(unsigned char *data);
```
Выгружает данные.

#### `SaveFileData`
```c
bool SaveFileData(const char *fileName, void *data, int dataSize);
```
Сохраняет данные в файл.

#### `ExportDataAsCode`
```c
bool ExportDataAsCode(const unsigned char *data, int dataSize, const char *fileName);
```
Экспорт как C-код.

#### `LoadFileText`
```c
char *LoadFileText(const char *fileName);
```
Загружает текст из файла.

#### `UnloadFileText`
```c
void UnloadFileText(char *text);
```
Выгружает текст.

#### `SaveFileText`
```c
bool SaveFileText(const char *fileName, char *text);
```
Сохраняет текст.

#### `FileExists`
```c
bool FileExists(const char *fileName);
```
Проверяет существование файла.

#### `DirectoryExists`
```c
bool DirectoryExists(const char *dirPath);
```
Проверяет существование директории.

#### `IsFileExtension`
```c
bool IsFileExtension(const char *fileName, const char *ext);
```
Проверяет расширение файла (с точкой: `.png`).

#### `GetFileLength`
```c
int GetFileLength(const char *fileName);
```
Размер файла в байтах.

#### `GetFileExtension`
```c
const char *GetFileExtension(const char *fileName);
```
Указатель на расширение.

#### `GetFileName`
```c
const char *GetFileName(const char *filePath);
```
Имя файла из пути.

#### `GetFileNameWithoutExt`
```c
const char *GetFileNameWithoutExt(const char *filePath);
```
Имя файла без расширения.

#### `GetDirectoryPath`
```c
const char *GetDirectoryPath(const char *filePath);
```
Путь к директории.

#### `GetPrevDirectoryPath`
```c
const char *GetPrevDirectoryPath(const char *dirPath);
```
Родительская директория.

#### `GetWorkingDirectory`
```c
const char *GetWorkingDirectory(void);
```
Текущая рабочая директория.

#### `GetApplicationDirectory`
```c
const char *GetApplicationDirectory(void);
```
Директория исполняемого файла.

#### `MakeDirectory`
```c
int MakeDirectory(const char *dirPath);
```
Создаёт директорию. Возвращает 0 при успехе.

#### `ChangeDirectory`
```c
bool ChangeDirectory(const char *dir);
```
Меняет рабочую директорию.

#### `IsPathFile`
```c
bool IsPathFile(const char *path);
```
Проверяет, является ли путь файлом.

#### `IsFileNameValid`
```c
bool IsFileNameValid(const char *fileName);
```
Проверяет, валидно ли имя файла.

#### `LoadDirectoryFiles`
```c
FilePathList LoadDirectoryFiles(const char *dirPath);
```
Загружает список файлов в директории.

#### `LoadDirectoryFilesEx`
```c
FilePathList LoadDirectoryFilesEx(const char *basePath, const char *filter, bool scanSubdirs);
```
Загружает файлы с фильтром.

#### `UnloadDirectoryFiles`
```c
void UnloadDirectoryFiles(FilePathList files);
```
Выгружает список файлов.

#### `IsFileDropped`
```c
bool IsFileDropped(void);
```
Проверяет, был ли файл брошен в окно.

#### `LoadDroppedFiles`
```c
FilePathList LoadDroppedFiles(void);
```
Загружает пути брошенных файлов.

#### `UnloadDroppedFiles`
```c
void UnloadDroppedFiles(FilePathList files);
```
Выгружает пути.

#### `GetFileModTime`
```c
long GetFileModTime(const char *fileName);
```
Время последнего изменения (unix timestamp).

### 3.11 Сжатие и кодирование

#### `CompressData` / `DecompressData` / `EncodeDataBase64` / `DecodeDataBase64`
Сжатие DEFLATE и кодирование Base64.

#### `ComputeCRC32` / `ComputeMD5` / `ComputeSHA1`
Вычисление хешей CRC32, MD5, SHA1.

### 3.12 Automation Events

#### `LoadAutomationEventList` / `UnloadAutomationEventList` / `ExportAutomationEventList`
#### `SetAutomationEventList` / `SetAutomationEventBaseFrame`
#### `StartAutomationEventRecording` / `StopAutomationEventRecording`
#### `PlayAutomationEvent`
Система записи и воспроизведения событий для автоматизации тестирования.

### 3.13 Ввод с клавиатуры (Keyboard Input)

#### `IsKeyPressed(int key)`
Была ли клавиша нажата один раз.

#### `IsKeyPressedRepeat(int key)`
Была ли клавиша нажата повторно (автоповтор).

#### `IsKeyDown(int key)`
Зажата ли клавиша (каждый кадр).

#### `IsKeyReleased(int key)`
Была ли отпущена.

#### `IsKeyUp(int key)`
Не зажата ли.

#### `GetKeyPressed(void)`
Возвращает код нажатой клавиши из очереди (0 = пусто).

#### `GetCharPressed(void)`
Возвращает Unicode-символ из очереди (учитывает раскладку).

#### `SetExitKey(int key)`
Устанавливает клавишу выхода (по умолчанию KEY_ESCAPE).

### 3.14 Ввод с геймпада (Gamepad Input)

#### `IsGamepadAvailable(int gamepad)`
Доступен ли геймпад.

#### `GetGamepadName(int gamepad)`
Имя геймпада.

#### `IsGamepadButtonPressed(int gamepad, int button)`
Нажата ли кнопка (один раз).

#### `IsGamepadButtonDown(int gamepad, int button)`
Зажата ли кнопка.

#### `IsGamepadButtonReleased(int gamepad, int button)`
Отпущена ли.

#### `IsGamepadButtonUp(int gamepad, int button)`
Не зажата ли.

#### `GetGamepadButtonPressed(void)`
Последняя нажатая кнопка.

#### `GetGamepadAxisCount(int gamepad)`
Количество осей.

#### `GetGamepadAxisMovement(int gamepad, int axis)`
Значение оси (-1.0 до 1.0).

#### `SetGamepadMappings(const char *mappings)`
Маппинги (SDL_GameControllerDB).

#### `SetGamepadVibration(int gamepad, float leftMotor, float rightMotor, float duration)`
Вибрация геймпада.

### 3.15 Ввод с мыши (Mouse Input)

#### `IsMouseButtonPressed(int button)`
Нажата ли кнопка (один раз). Кнопки: `MOUSE_BUTTON_LEFT`, `_RIGHT`, `_MIDDLE`, `_SIDE`, `_EXTRA`, `_FORWARD`, `_BACK`.

#### `IsMouseButtonDown / IsMouseButtonReleased / IsMouseButtonUp`
Состояние кнопки мыши.

#### `GetMouseX() / GetMouseY() / GetMousePosition()`
Позиция мыши.

#### `GetMouseDelta()`
Изменение позиции между кадрами.

#### `SetMousePosition(x, y)`
Устанавливает позицию.

#### `SetMouseOffset(offsetX, offsetY)`
Смещение.

#### `SetMouseScale(scaleX, scaleY)`
Масштаб.

#### `GetMouseWheelMove()`
Движение колеса (float).

#### `GetMouseWheelMoveV()`
Движение колеса (Vector2).

#### `SetMouseCursor(int cursor)`
Курсор мыши.

### 3.16 Ввод с тачскрина (Touch Input)

#### `GetTouchX() / GetTouchY() / GetTouchPosition(index)`
#### `GetTouchPointId(index) / GetTouchPointCount()`
Касания.

### 3.17 Жесты (Gestures)

#### `SetGesturesEnabled(unsigned int flags)`
Включает жесты.

#### `IsGestureDetected(unsigned int gesture)`
#### `GetGestureDetected()`
#### `GetGestureHoldDuration()`
#### `GetGestureDragVector() / GetGestureDragAngle()`
#### `GetGesturePinchVector() / GetGesturePinchAngle()`

### 3.18 Система камер (Camera System)

#### `UpdateCamera(Camera *camera, int mode)`
Обновляет камеру для выбранного режима: `CAMERA_CUSTOM`, `CAMERA_FREE`, `CAMERA_ORBITAL`, `CAMERA_FIRST_PERSON`, `CAMERA_THIRD_PERSON`.

#### `UpdateCameraPro(Camera *camera, Vector3 movement, Vector3 rotation, float zoom)`
Ручное управление камерой.

### 3.19 Базовые 2D фигуры

`SetShapesTexture`, `GetShapesTexture`, `GetShapesTextureRectangle`

#### `DrawPixel(posX, posY, color)` / `DrawPixelV(position, color)`

#### `DrawLine(x1, y1, x2, y2, color)` / `DrawLineV(start, end, color)` / `DrawLineEx(start, end, thick, color)` / `DrawLineStrip(points, count, color)` / `DrawLineBezier(start, end, thick, color)`

#### `DrawCircle(x, y, radius, color)` / `DrawCircleV(center, radius, color)` / `DrawCircleLines(x, y, radius, color)` / `DrawCircleLinesV(center, radius, color)`
#### `DrawCircleSector(center, radius, startAngle, endAngle, segments, color)`
#### `DrawCircleSectorLines(center, radius, startAngle, endAngle, segments, color)`
#### `DrawCircleGradient(x, y, radius, inner, outer)`

#### `DrawEllipse(x, y, radiusH, radiusV, color)` / `DrawEllipseLines(...)`

#### `DrawRing(center, innerRadius, outerRadius, startAngle, endAngle, segments, color)`
#### `DrawRingLines(...)`

#### `DrawRectangle(x, y, w, h, color)` / `DrawRectangleV(pos, size, color)` / `DrawRectangleRec(rec, color)`
#### `DrawRectanglePro(rec, origin, rotation, color)`
#### `DrawRectangleGradientV(x, y, w, h, top, bottom)` / `DrawRectangleGradientH(x, y, w, h, left, right)`
#### `DrawRectangleGradientEx(rec, colTopLeft, colBotLeft, colTopRight, colBotRight)`
#### `DrawRectangleLines(x, y, w, h, color)` / `DrawRectangleLinesEx(rec, lineThick, color)`
#### `DrawRectangleRounded(rec, roundness, segments, color)` / `DrawRectangleRoundedLines(rec, roundness, segments, color)` / `DrawRectangleRoundedLinesEx(rec, roundness, segments, lineThick, color)`

#### `DrawTriangle(v1, v2, v3, color)` / `DrawTriangleLines(v1, v2, v3, color)`
#### `DrawTriangleFan(points, count, color)` / `DrawTriangleStrip(points, count, color)`

#### `DrawPoly(center, sides, radius, rotation, color)`
#### `DrawPolyLines(center, sides, radius, rotation, color)`
#### `DrawPolyLinesEx(center, sides, radius, rotation, lineThick, color)`

### 3.20 Сплайны (Splines)

#### `DrawSplineLinear(points, pointCount, thick, color)` — минимум 2 точки
#### `DrawSplineBasis(points, pointCount, thick, color)` — минимум 4 точки
#### `DrawSplineCatmullRom(points, pointCount, thick, color)` — минимум 4 точки
#### `DrawSplineBezierQuadratic(points, pointCount, thick, color)` — минимум 3 точки
#### `DrawSplineBezierCubic(points, pointCount, thick, color)` — минимум 4 точки

#### `DrawSplineSegmentLinear(p1, p2, thick, color)`
#### `DrawSplineSegmentBasis(p1, p2, p3, p4, thick, color)`
#### `DrawSplineSegmentCatmullRom(p1, p2, p3, p4, thick, color)`
#### `DrawSplineSegmentBezierQuadratic(p1, c2, p3, thick, color)`
#### `DrawSplineSegmentBezierCubic(p1, c2, c3, p4, thick, color)`

#### `GetSplinePointLinear(startPos, endPos, t)`
#### `GetSplinePointBasis(p1, p2, p3, p4, t)`
#### `GetSplinePointCatmullRom(p1, p2, p3, p4, t)`
#### `GetSplinePointBezierQuad(p1, c2, p3, t)`
#### `GetSplinePointBezierCubic(p1, c2, c3, p4, t)`
Вычисление точки на сплайне (t = 0.0..1.0).

### 3.21 2D Коллизии (Collision Detection)

#### `CheckCollisionRecs(rec1, rec2)` — два прямоугольника
#### `CheckCollisionCircles(center1, r1, center2, r2)` — два круга
#### `CheckCollisionCircleRec(center, radius, rec)` — круг и прямоугольник
#### `CheckCollisionCircleLine(center, radius, p1, p2)` — круг и линия
#### `CheckCollisionPointRec(point, rec)` — точка в прямоугольнике
#### `CheckCollisionPointCircle(point, center, radius)` — точка в круге
#### `CheckCollisionPointTriangle(point, p1, p2, p3)` — точка в треугольнике
#### `CheckCollisionPointLine(point, p1, p2, threshold)` — точка на линии
#### `CheckCollisionPointPoly(point, points, pointCount)` — точка в многоугольнике
#### `CheckCollisionLines(p1, p2, p3, p4, *collisionPoint)` — пересечение линий
#### `GetCollisionRec(rec1, rec2)` — прямоугольник пересечения

### 3.22 Изображения (Image Loading)

#### `LoadImage(fileName)` / `LoadImageRaw(fileName, width, height, format, headerSize)`
#### `LoadImageAnim(fileName, *frames)` / `LoadImageAnimFromMemory(fileType, fileData, dataSize, *frames)`
#### `LoadImageFromMemory(fileType, fileData, dataSize)`
#### `LoadImageFromTexture(texture)` — из GPU в CPU
#### `LoadImageFromScreen()` — скриншот
#### `IsImageValid(image)` / `UnloadImage(image)`
#### `ExportImage(image, fileName)` / `ExportImageToMemory(image, fileType, *fileSize)` / `ExportImageAsCode(image, fileName)`

### 3.23 Генерация изображений

#### `GenImageColor(w, h, color)` — одноцветное
#### `GenImageGradientLinear(w, h, direction, start, end)` — линейный градиент
#### `GenImageGradientRadial(w, h, density, inner, outer)` — радиальный градиент
#### `GenImageGradientSquare(w, h, density, inner, outer)` — квадратный градиент
#### `GenImageChecked(w, h, checksX, checksY, col1, col2)` — шахматное
#### `GenImageWhiteNoise(w, h, factor)` — белый шум
#### `GenImagePerlinNoise(w, h, offsetX, offsetY, scale)` — шум Перлина
#### `GenImageCellular(w, h, tileSize)` — клеточное
#### `GenImageText(w, h, text)` — из текста

### 3.24 Манипуляции с изображениями

#### `ImageCopy(image)` — дубликат
#### `ImageFromImage(image, rec)` — часть изображения
#### `ImageFromChannel(image, selectedChannel)` — канал в grayscale
#### `ImageText(text, fontSize, color)` / `ImageTextEx(font, text, fontSize, spacing, tint)` — из текста
#### `ImageFormat(image, newFormat)` — конвертация формата
#### `ImageToPOT(image, fill)` — степень двойки
#### `ImageCrop(image, crop)` — обрезка
#### `ImageAlphaCrop(image, threshold)` — обрезка по альфе
#### `ImageAlphaClear(image, color, threshold)` — альфа-канал
#### `ImageAlphaMask(image, alphaMask)` — альфа-маска
#### `ImageAlphaPremultiply(image)` — premultiply
#### `ImageBlurGaussian(image, blurSize)` — размытие
#### `ImageKernelConvolution(image, kernel, kernelSize)` — свёртка
#### `ImageResize(image, newW, newH)` / `ImageResizeNN(image, newW, newH)` — изменение размера
#### `ImageResizeCanvas(image, newW, newH, offsetX, offsetY, fill)` — холст
#### `ImageMipmaps(image)` — mipmap
#### `ImageDither(image, rBpp, gBpp, bBpp, aBpp)` — дизеринг
#### `ImageFlipVertical(image)` / `ImageFlipHorizontal(image)`
#### `ImageRotate(image, degrees)` / `ImageRotateCW(image)` / `ImageRotateCCW(image)`
#### `ImageColorTint(image, color)` / `ImageColorInvert(image)` / `ImageColorGrayscale(image)`
#### `ImageColorContrast(image, contrast)` / `ImageColorBrightness(image, brightness)` / `ImageColorReplace(image, color, replace)`
#### `LoadImageColors(image)` / `LoadImagePalette(image, maxPaletteSize, *colorCount)`
#### `UnloadImageColors(colors)` / `UnloadImagePalette(colors)`
#### `GetImageAlphaBorder(image, threshold)` / `GetImageColor(image, x, y)`

### 3.25 Рисование на изображениях

#### `ImageClearBackground(dst, color)`
#### `ImageDrawPixel(dst, posX, posY, color)` / V-версия
#### `ImageDrawLine(dst, x1, y1, x2, y2, color)` / V / Ex
#### `ImageDrawCircle(dst, x, y, radius, color)` / V / CircleLines / CircleLinesV
#### `ImageDrawRectangle(dst, x, y, w, h, color)` / V / Rec / Lines
#### `ImageDrawTriangle(dst, v1, v2, v3, color)` / Ex / Lines / Fan / Strip
#### `ImageDraw(dst, src, srcRec, dstRec, tint)` — одно изображение на другом
#### `ImageDrawText(dst, text, posX, posY, fontSize, color)` / `ImageDrawTextEx(dst, font, text, pos, fontSize, spacing, tint)`

### 3.26 Текстуры (Texture Loading and Drawing)

#### `LoadTexture(fileName)` — загрузка в GPU
#### `LoadTextureFromImage(image)` — из Image
#### `LoadTextureCubemap(image, layout)`
#### `LoadRenderTexture(w, h)` — для рендера (FBO)
#### `IsTextureValid(texture)` / `UnloadTexture(texture)`
#### `IsRenderTextureValid(target)` / `UnloadRenderTexture(target)`
#### `UpdateTexture(texture, pixels)` / `UpdateTextureRec(texture, rec, pixels)`
#### `GenTextureMipmaps(texture)` / `SetTextureFilter(texture, filter)` / `SetTextureWrap(texture, wrap)`

#### `DrawTexture(texture, posX, posY, tint)` / `DrawTextureV(texture, pos, tint)` / `DrawTextureEx(texture, pos, rot, scale, tint)`
#### `DrawTextureRec(texture, source, pos, tint)` — часть текстуры
#### `DrawTexturePro(texture, source, dest, origin, rotation, tint)` — с полными параметрами
#### `DrawTextureNPatch(texture, nPatchInfo, dest, origin, rotation, tint)` — 9-patch

### 3.27 Цвет (Color Functions)

#### `ColorIsEqual(col1, col2)` / `Fade(color, alpha)` / `ColorToInt(color)`
#### `ColorNormalize(color)` — [0..1] / `ColorFromNormalized(normalized)`
#### `ColorToHSV(color)` / `ColorFromHSV(hue, sat, val)`
#### `ColorTint(color, tint)` / `ColorBrightness(color, factor)` / `ColorContrast(color, contrast)`
#### `ColorAlpha(color, alpha)` / `ColorAlphaBlend(dst, src, tint)` / `ColorLerp(c1, c2, factor)`
#### `GetColor(hexValue)` / `GetPixelColor(srcPtr, format)` / `SetPixelColor(dstPtr, color, format)`
#### `GetPixelDataSize(width, height, format)`

### Константы цветов

```
LIGHTGRAY  (200,200,200,255)  GRAY    (130,130,130,255)  DARKGRAY (80,80,80,255)
YELLOW    (253,249,0,255)    GOLD    (255,203,0,255)    ORANGE  (255,161,0,255)
PINK      (255,109,194,255)  RED     (230,41,55,255)    MAROON  (190,33,55,255)
GREEN     (0,228,48,255)     LIME    (0,158,47,255)     DARKGREEN (0,117,44,255)
SKYBLUE   (102,191,255,255)  BLUE    (0,121,241,255)    DARKBLUE (0,82,172,255)
PURPLE    (200,122,255,255)  VIOLET  (135,60,190,255)   DARKPURPLE (112,31,126,255)
BEIGE     (211,176,131,255)  BROWN   (127,106,79,255)   DARKBROWN (76,63,47,255)
WHITE     (255,255,255,255)  BLACK   (0,0,0,255)        BLANK   (0,0,0,0)
MAGENTA   (255,0,255,255)    RAYWHITE(245,245,245,255)
```

### 3.28 Шрифты и текст (Font and Text)

#### `GetFontDefault()` / `LoadFont(fileName)` / `LoadFontEx(fileName, fontSize, codepoints, codepointCount)`
#### `LoadFontFromImage(image, key, firstChar)` / `LoadFontFromMemory(fileType, data, dataSize, fontSize, codepoints, count)`
#### `IsFontValid(font)` / `LoadFontData(data, size, fontSize, codepoints, count, type)` / `UnloadFontData(glyphs, count)`
#### `GenImageFontAtlas(glyphs, recs, count, fontSize, padding, packMethod)`
#### `UnloadFont(font)` / `ExportFontAsCode(font, fileName)`

#### `DrawFPS(posX, posY)`
#### `DrawText(text, posX, posY, fontSize, color)` — стандартный шрифт
#### `DrawTextEx(font, text, pos, fontSize, spacing, tint)` — кастомный шрифт
#### `DrawTextPro(font, text, pos, origin, rotation, fontSize, spacing, tint)` — с поворотом
#### `DrawTextCodepoint(font, codepoint, pos, fontSize, tint)` / `DrawTextCodepoints(font, codepoints, count, pos, fontSize, spacing, tint)`

#### `SetTextLineSpacing(spacing)`
#### `MeasureText(text, fontSize)` — ширина (int)
#### `MeasureTextEx(font, text, fontSize, spacing)` — размер (Vector2)
#### `GetGlyphIndex(font, codepoint)` / `GetGlyphInfo(font, codepoint)` / `GetGlyphAtlasRec(font, codepoint)`

#### `LoadUTF8(codepoints, length)` / `UnloadUTF8(text)`
#### `LoadCodepoints(text, *count)` / `UnloadCodepoints(codepoints)`
#### `GetCodepointCount(text)` / `GetCodepoint(text, *size)` / `GetCodepointNext/Previous(text, *size)`
#### `CodepointToUTF8(codepoint, *utf8Size)`

#### `TextCopy(dst, src)` / `TextIsEqual(t1, t2)` / `TextLength(text)`
#### `TextFormat(text, ...)` — форматирование (sprintf)
#### `TextSubtext(text, pos, len)` / `TextReplace(text, find, replace)` / `TextInsert(text, insert, pos)`
#### `TextJoin(list, count, delimiter)` / `TextSplit(text, delimiter, *count)`
#### `TextAppend(text, append, *pos)` / `TextFindIndex(text, find)`
#### `TextToUpper(text)` / `TextToLower(text)` / `TextToPascal(text)` / `TextToSnake(text)` / `TextToCamel(text)`
#### `TextToInteger(text)` / `TextToFloat(text)`

### 3.29 3D Фигуры (3D Shapes Drawing)

#### `DrawLine3D(startPos, endPos, color)`
#### `DrawPoint3D(position, color)`
#### `DrawCircle3D(center, radius, rotationAxis, rotationAngle, color)`
#### `DrawTriangle3D(v1, v2, v3, color)` — против часовой стрелки
#### `DrawTriangleStrip3D(points, pointCount, color)`
#### `DrawCube(pos, w, h, l, color)` / `DrawCubeV(pos, size, color)`
#### `DrawCubeWires(pos, w, h, l, color)` / `DrawCubeWiresV(pos, size, color)`
#### `DrawSphere(centerPos, radius, color)` / `DrawSphereEx(pos, radius, rings, slices, color)` / `DrawSphereWires(pos, radius, rings, slices, color)`
#### `DrawCylinder(pos, radiusTop, radiusBottom, height, slices, color)` / `DrawCylinderEx(startPos, endPos, startR, endR, sides, color)`
#### `DrawCylinderWires(...)` / `DrawCylinderWiresEx(...)`
#### `DrawCapsule(startPos, endPos, radius, slices, rings, color)` / `DrawCapsuleWires(...)`
#### `DrawPlane(centerPos, size, color)` — плоскость XZ
#### `DrawRay(ray, color)`
#### `DrawGrid(slices, spacing)` — сетка

### 3.30 Модели (Model Loading and Drawing)

#### `LoadModel(fileName)` / `LoadModelFromMesh(mesh)` / `IsModelValid(model)` / `UnloadModel(model)`
#### `GetModelBoundingBox(model)`
#### `DrawModel(model, pos, scale, tint)` / `DrawModelEx(model, pos, rotAxis, rotAngle, scale, tint)`
#### `DrawModelWires(model, pos, scale, tint)` / `DrawModelWiresEx(...)`
#### `DrawModelPoints(model, pos, scale, tint)` / `DrawModelPointsEx(...)`
#### `DrawBoundingBox(box, color)`
#### `DrawBillboard(camera, texture, pos, scale, tint)` / `DrawBillboardRec(camera, texture, source, pos, size, tint)` / `DrawBillboardPro(camera, texture, source, pos, up, size, origin, rot, tint)`

### 3.31 Меши (Mesh Management)

#### `UploadMesh(mesh, dynamic)` — загрузка на GPU
#### `UpdateMeshBuffer(mesh, index, data, size, offset)`
#### `UnloadMesh(mesh)`
#### `DrawMesh(mesh, material, transform)` / `DrawMeshInstanced(mesh, material, transforms, instances)`
#### `GetMeshBoundingBox(mesh)` / `GenMeshTangents(mesh)`
#### `ExportMesh(mesh, fileName)` / `ExportMeshAsCode(mesh, fileName)`
#### `GenMeshPoly(sides, radius)` / `GenMeshPlane(w, l, resX, resZ)` / `GenMeshCube(w, h, l)`
#### `GenMeshSphere(radius, rings, slices)` / `GenMeshHemiSphere(radius, rings, slices)`
#### `GenMeshCylinder(radius, height, slices)` / `GenMeshCone(radius, height, slices)`
#### `GenMeshTorus(radius, size, radSeg, sides)` / `GenMeshKnot(radius, size, radSeg, sides)`
#### `GenMeshHeightmap(heightmap, size)` / `GenMeshCubicmap(cubicmap, cubeSize)`

### 3.32 Материалы (Material)

#### `LoadMaterials(fileName, *count)` / `LoadMaterialDefault()` / `IsMaterialValid(material)` / `UnloadMaterial(material)`
#### `SetMaterialTexture(material, mapType, texture)` / `SetModelMeshMaterial(model, meshId, materialId)`

### 3.33 Анимации моделей

#### `LoadModelAnimations(fileName, *count)` / `UnloadModelAnimation(anim)` / `UnloadModelAnimations(anims, count)`
#### `UpdateModelAnimation(model, anim, frame)` / `UpdateModelAnimationBones(model, anim, frame)`
#### `IsModelAnimationValid(model, anim)`

### 3.34 3D Коллизии

#### `CheckCollisionSpheres(c1, r1, c2, r2)` / `CheckCollisionBoxes(b1, b2)` / `CheckCollisionBoxSphere(box, center, radius)`
#### `GetRayCollisionSphere(ray, center, radius)` / `GetRayCollisionBox(ray, box)` / `GetRayCollisionMesh(ray, mesh, transform)`
#### `GetRayCollisionTriangle(ray, p1, p2, p3)` / `GetRayCollisionQuad(ray, p1, p2, p3, p4)`

### 3.35 Аудио — устройство

#### `InitAudioDevice()` / `CloseAudioDevice()` / `IsAudioDeviceReady()`
#### `SetMasterVolume(volume)` / `GetMasterVolume()`

### 3.36 Аудио — волны и звуки

#### `LoadWave(fileName)` / `LoadWaveFromMemory(fileType, data, dataSize)` / `IsWaveValid(wave)` / `UnloadWave(wave)`
#### `LoadSound(fileName)` / `LoadSoundFromWave(wave)` / `LoadSoundAlias(source)` / `IsSoundValid(sound)` / `UnloadSound(sound)` / `UnloadSoundAlias(alias)`
#### `UpdateSound(sound, data, sampleCount)`
#### `ExportWave(wave, fileName)` / `ExportWaveAsCode(wave, fileName)`

### 3.37 Аудио — воспроизведение

#### `PlaySound(sound)` / `StopSound(sound)` / `PauseSound(sound)` / `ResumeSound(sound)` / `IsSoundPlaying(sound)`
#### `SetSoundVolume(sound, volume)` / `SetSoundPitch(sound, pitch)` / `SetSoundPan(sound, pan)`
#### `WaveCopy(wave)` / `WaveCrop(wave, initFrame, finalFrame)` / `WaveFormat(wave, sampleRate, sampleSize, channels)`
#### `LoadWaveSamples(wave)` / `UnloadWaveSamples(samples)`

### 3.38 Аудио — музыка

#### `LoadMusicStream(fileName)` / `LoadMusicStreamFromMemory(fileType, data, dataSize)` / `IsMusicValid(music)` / `UnloadMusicStream(music)`
#### `PlayMusicStream(music)` / `IsMusicStreamPlaying(music)` / `UpdateMusicStream(music)` (каждый кадр!)
#### `StopMusicStream(music)` / `PauseMusicStream(music)` / `ResumeMusicStream(music)`
#### `SeekMusicStream(music, position)` / `SetMusicVolume(music, volume)` / `SetMusicPitch(music, pitch)` / `SetMusicPan(music, pan)`
#### `GetMusicTimeLength(music)` / `GetMusicTimePlayed(music)`

### 3.39 Аудио — аудиопотоки

#### `LoadAudioStream(sampleRate, sampleSize, channels)` / `IsAudioStreamValid(stream)` / `UnloadAudioStream(stream)`
#### `UpdateAudioStream(stream, data, frameCount)` / `IsAudioStreamProcessed(stream)`
#### `PlayAudioStream(stream)` / `PauseAudioStream(stream)` / `ResumeAudioStream(stream)` / `IsAudioStreamPlaying(stream)` / `StopAudioStream(stream)`
#### `SetAudioStreamVolume(stream, volume)` / `SetAudioStreamPitch(stream, pitch)` / `SetAudioStreamPan(stream, pan)`
#### `SetAudioStreamBufferSizeDefault(size)` / `SetAudioStreamCallback(stream, callback)`
#### `AttachAudioStreamProcessor(stream, processor)` / `DetachAudioStreamProcessor(stream, processor)`
#### `AttachAudioMixedProcessor(processor)` / `DetachAudioMixedProcessor(processor)`

---

## 4. Типы данных (структуры и перечисления)

### 4.1 Структуры

#### `Vector2` — 2D вектор (x, y: float)
#### `Vector3` — 3D вектор (x, y, z: float)
#### `Vector4` — 4D вектор (x, y, z, w: float)
#### `Quaternion` — псевдоним Vector4
#### `Matrix` — 4x4, column-major (m0-m15: float)
#### `Color` — RGBA (r, g, b, a: unsigned char)
#### `Rectangle` — (x, y, width, height: float)
#### `Image` — (data: void*, width, height, mipmaps, format: int)
#### `Texture` / Texture2D / TextureCubemap — (id: uint, width, height, mipmaps, format: int)
#### `RenderTexture` / RenderTexture2D — (id: uint, texture: Texture, depth: Texture)
#### `NPatchInfo` — (source: Rect, left, top, right, bottom: int, layout: int)
#### `GlyphInfo` — (value: int, offsetX/Y: int, advanceX: int, image: Image)
#### `Font` — (baseSize, glyphCount, glyphPadding: int, texture: Texture2D, recs: Rect*, glyphs: GlyphInfo*)
#### `Camera3D` / Camera — (position, target, up: Vector3, fovy: float, projection: int)
#### `Camera2D` — (offset, target: Vector2, rotation: float, zoom: float)
#### `Mesh` — (vertexCount, triangleCount: int; vertices, texcoords, normals, tangents, colors, indices и др.; vaoId, vboId)
#### `Shader` — (id: uint, locs: int*)
#### `MaterialMap` — (texture: Texture2D, color: Color, value: float)
#### `Material` — (shader: Shader, maps: MaterialMap*, params: float[4])
#### `Transform` — (translation: Vector3, rotation: Quaternion, scale: Vector3)
#### `BoneInfo` — (name: char[32], parent: int)
#### `Model` — (transform: Matrix; meshCount, materialCount: int; meshes, materials, meshMaterial; boneCount; bones, bindPose)
#### `ModelAnimation` — (boneCount, frameCount: int; bones: BoneInfo*; framePoses: Transform**; name: char[32])
#### `Ray` — (position, direction: Vector3)
#### `RayCollision` — (hit: bool; distance: float; point, normal: Vector3)
#### `BoundingBox` — (min, max: Vector3)
#### `Wave` — (frameCount, sampleRate, sampleSize, channels: uint; data: void*)
#### `AudioStream` — (buffer, processor: указатели; sampleRate, sampleSize, channels: uint)
#### `Sound` — (stream: AudioStream, frameCount: uint)
#### `Music` — (stream: AudioStream; frameCount: uint; looping: bool; ctxType: int; ctxData: void*)
#### `VrDeviceInfo` — параметры VR HMD
#### `VrStereoConfig` — конфигурация VR стерео
#### `FilePathList` — (capacity, count: uint; paths: char**)
#### `AutomationEvent` — (frame, type: uint; params: int[4])
#### `AutomationEventList` — (capacity, count: uint; events: AutomationEvent*)

### 4.2 Перечисления (Enums)

#### `ConfigFlags`
`FLAG_VSYNC_HINT`, `FLAG_FULLSCREEN_MODE`, `FLAG_WINDOW_RESIZABLE`, `FLAG_WINDOW_UNDECORATED`, `FLAG_WINDOW_HIDDEN`, `FLAG_WINDOW_MINIMIZED`, `FLAG_WINDOW_MAXIMIZED`, `FLAG_WINDOW_UNFOCUSED`, `FLAG_WINDOW_TOPMOST`, `FLAG_WINDOW_ALWAYS_RUN`, `FLAG_WINDOW_TRANSPARENT`, `FLAG_WINDOW_HIGHDPI`, `FLAG_WINDOW_MOUSE_PASSTHROUGH`, `FLAG_BORDERLESS_WINDOWED_MODE`, `FLAG_MSAA_4X_HINT`, `FLAG_INTERLACED_HINT`

#### `TraceLogLevel`
`LOG_ALL`, `LOG_TRACE`, `LOG_DEBUG`, `LOG_INFO`, `LOG_WARNING`, `LOG_ERROR`, `LOG_FATAL`, `LOG_NONE`

#### `KeyboardKey` (все клавиши)
`KEY_NULL`, `KEY_APOSTROPHE`(39), `KEY_COMMA`(44), `KEY_MINUS`(45), `KEY_PERIOD`(46), `KEY_SLASH`(47),
`KEY_ZERO`-`KEY_NINE`(48-57), `KEY_SEMICOLON`(59), `KEY_EQUAL`(61),
`KEY_A`-`KEY_Z`(65-90), `KEY_LEFT_BRACKET`(91), `KEY_BACKSLASH`(92), `KEY_RIGHT_BRACKET`(93), `KEY_GRAVE`(96),
`KEY_SPACE`(32), `KEY_ESCAPE`(256), `KEY_ENTER`(257), `KEY_TAB`(258), `KEY_BACKSPACE`(259),
`KEY_INSERT`(260), `KEY_DELETE`(261), `KEY_RIGHT`(262), `KEY_LEFT`(263), `KEY_DOWN`(264), `KEY_UP`(265),
`KEY_PAGE_UP`(266), `KEY_PAGE_DOWN`(267), `KEY_HOME`(268), `KEY_END`(269),
`KEY_CAPS_LOCK`(280), `KEY_SCROLL_LOCK`(281), `KEY_NUM_LOCK`(282), `KEY_PRINT_SCREEN`(283), `KEY_PAUSE`(284),
`KEY_F1`-`KEY_F12`(290-301),
`KEY_LEFT_SHIFT`(340), `KEY_LEFT_CONTROL`(341), `KEY_LEFT_ALT`(342), `KEY_LEFT_SUPER`(343),
`KEY_RIGHT_SHIFT`(344), `KEY_RIGHT_CONTROL`(345), `KEY_RIGHT_ALT`(346), `KEY_RIGHT_SUPER`(347), `KEY_KB_MENU`(348),
`KEY_KP_0`-`KEY_KP_9`(320-329), `KEY_KP_DECIMAL`(330), `KEY_KP_DIVIDE`(331), `KEY_KP_MULTIPLY`(332),
`KEY_KP_SUBTRACT`(333), `KEY_KP_ADD`(334), `KEY_KP_ENTER`(335), `KEY_KP_EQUAL`(336),
`KEY_BACK`(4), `KEY_MENU`(5), `KEY_VOLUME_UP`(24), `KEY_VOLUME_DOWN`(25)

#### `MouseButton`
`MOUSE_BUTTON_LEFT`(0), `_RIGHT`(1), `_MIDDLE`(2), `_SIDE`(3), `_EXTRA`(4), `_FORWARD`(5), `_BACK`(6)

#### `MouseCursor`
`MOUSE_CURSOR_DEFAULT`(0), `_ARROW`(1), `_IBEAM`(2), `_CROSSHAIR`(3), `_POINTING_HAND`(4),
`_RESIZE_EW`(5), `_RESIZE_NS`(6), `_RESIZE_NWSE`(7), `_RESIZE_NESW`(8), `_RESIZE_ALL`(9), `_NOT_ALLOWED`(10)

#### `GamepadButton`
`GAMEPAD_BUTTON_UNKNOWN`, `_LEFT_FACE_UP`, `_LEFT_FACE_RIGHT`, `_LEFT_FACE_DOWN`, `_LEFT_FACE_LEFT`,
`_RIGHT_FACE_UP`(Triangle/Y), `_RIGHT_FACE_RIGHT`(Circle/B), `_RIGHT_FACE_DOWN`(Cross/A), `_RIGHT_FACE_LEFT`(Square/X),
`_LEFT_TRIGGER_1`, `_LEFT_TRIGGER_2`, `_RIGHT_TRIGGER_1`, `_RIGHT_TRIGGER_2`,
`_MIDDLE_LEFT`, `_MIDDLE`, `_MIDDLE_RIGHT`, `_LEFT_THUMB`, `_RIGHT_THUMB`

#### `GamepadAxis`
`GAMEPAD_AXIS_LEFT_X`, `_LEFT_Y`, `_RIGHT_X`, `_RIGHT_Y`, `_LEFT_TRIGGER`, `_RIGHT_TRIGGER`

#### `MaterialMapIndex`
`MATERIAL_MAP_ALBEDO`(0), `_METALNESS`(1), `_NORMAL`(2), `_ROUGHNESS`(3), `_OCCLUSION`(4),
`_EMISSION`(5), `_HEIGHT`(6), `_CUBEMAP`(7), `_IRRADIANCE`(8), `_PREFILTER`(9), `_BRDF`(10)
Алиасы: `MATERIAL_MAP_DIFFUSE = ALBEDO`, `MATERIAL_MAP_SPECULAR = METALNESS`

#### `ShaderLocationIndex`
`SHADER_LOC_VERTEX_POSITION`(0), `_TEXCOORD01`, `_TEXCOORD02`, `_VERTEX_NORMAL`, `_VERTEX_TANGENT`, `_VERTEX_COLOR`,
`_MATRIX_MVP`, `_MATRIX_VIEW`, `_MATRIX_PROJECTION`, `_MATRIX_MODEL`, `_MATRIX_NORMAL`, `_VECTOR_VIEW`,
`_COLOR_DIFFUSE`, `_COLOR_SPECULAR`, `_COLOR_AMBIENT`,
`_MAP_ALBEDO`, `_MAP_METALNESS`, `_MAP_NORMAL`, `_MAP_ROUGHNESS`, `_MAP_OCCLUSION`, `_MAP_EMISSION`, `_MAP_HEIGHT`,
`_MAP_CUBEMAP`, `_MAP_IRRADIANCE`, `_MAP_PREFILTER`, `_MAP_BRDF`,
`_VERTEX_BONEIDS`, `_VERTEX_BONEWEIGHTS`, `_BONE_MATRICES`
Алиасы: `SHADER_LOC_MAP_DIFFUSE = MAP_ALBEDO`, `SHADER_LOC_MAP_SPECULAR = MAP_METALNESS`

#### `ShaderUniformDataType`
`SHADER_UNIFORM_FLOAT`(0), `_VEC2`, `_VEC3`, `_VEC4`, `_INT`, `_IVEC2`, `_IVEC3`, `_IVEC4`, `_SAMPLER2D`

#### `ShaderAttributeDataType`
`SHADER_ATTRIB_FLOAT`, `SHADER_ATTRIB_VEC2`, `SHADER_ATTRIB_VEC3`, `SHADER_ATTRIB_VEC4`

#### `PixelFormat`
`PIXELFORMAT_UNCOMPRESSED_GRAYSCALE`(1), `_GRAY_ALPHA`, `_R5G6B5`, `_R8G8B8`, `_R5G5B5A1`, `_R4G4B4A4`, `_R8G8B8A8`,
`_R32`, `_R32G32B32`, `_R32G32B32A32`, `_R16`, `_R16G16B16`, `_R16G16B16A16`,
`_COMPRESSED_DXT1_RGB`, `_DXT1_RGBA`, `_DXT3_RGBA`, `_DXT5_RGBA`,
`_ETC1_RGB`, `_ETC2_RGB`, `_ETC2_EAC_RGBA`, `_PVRT_RGB`, `_PVRT_RGBA`,
`_ASTC_4x4_RGBA`, `_ASTC_8x8_RGBA`

#### `TextureFilter`
`TEXTURE_FILTER_POINT`(0), `_BILINEAR`, `_TRILINEAR`, `_ANISOTROPIC_4X`, `_ANISOTROPIC_8X`, `_ANISOTROPIC_16X`

#### `TextureWrap`
`TEXTURE_WRAP_REPEAT`(0), `_CLAMP`, `_MIRROR_REPEAT`, `_MIRROR_CLAMP`

#### `CubemapLayout`
`CUBEMAP_LAYOUT_AUTO_DETECT`, `_LINE_VERTICAL`, `_LINE_HORIZONTAL`, `_CROSS_THREE_BY_FOUR`, `_CROSS_FOUR_BY_THREE`

#### `FontType`
`FONT_DEFAULT`(0), `FONT_BITMAP`, `FONT_SDF`

#### `BlendMode`
`BLEND_ALPHA`(0), `_ADDITIVE`, `_MULTIPLIED`, `_ADD_COLORS`, `_SUBTRACT_COLORS`, `_ALPHA_PREMULTIPLY`, `_CUSTOM`, `_CUSTOM_SEPARATE`

#### `Gesture`
`GESTURE_NONE`(0), `_TAP`(1), `_DOUBLETAP`(2), `_HOLD`(4), `_DRAG`(8),
`_SWIPE_RIGHT`(16), `_SWIPE_LEFT`(32), `_SWIPE_UP`(64), `_SWIPE_DOWN`(128),
`_PINCH_IN`(256), `_PINCH_OUT`(512)

#### `CameraMode`
`CAMERA_CUSTOM`(0), `CAMERA_FREE`, `CAMERA_ORBITAL`, `CAMERA_FIRST_PERSON`, `CAMERA_THIRD_PERSON`

#### `CameraProjection`
`CAMERA_PERSPECTIVE`(0), `CAMERA_ORTHOGRAPHIC`

#### `NPatchLayout`
`NPATCH_NINE_PATCH`(0), `NPATCH_THREE_PATCH_VERTICAL`, `NPATCH_THREE_PATCH_HORIZONTAL`

### Типы колбэков

```c
typedef void (*TraceLogCallback)(int logLevel, const char *text, va_list args);
typedef unsigned char *(*LoadFileDataCallback)(const char *fileName, int *dataSize);
typedef bool (*SaveFileDataCallback)(const char *fileName, void *data, int dataSize);
typedef char *(*LoadFileTextCallback)(const char *fileName);
typedef bool (*SaveFileTextCallback)(const char *fileName, char *text);
```

---

## 5. Полный разбор примера (main.cpp)

```cpp
#include "raylib.h"
```
Подключает заголовочный файл raylib. Содержит все объявления функций, структур, констант. Компилятор ищет `raylib.h` в путях include, которые передаёт CMake.

```cpp
int main() {
```
Точка входа в программу.

```cpp
    const int screenWidth = 800;
    const int screenHeight = 600;
```
Объявляет константы ширины и высоты окна. `const` — значение не изменится.

```cpp
    InitWindow(screenWidth, screenHeight, "Robot");
```
Инициализирует окно размером 800x600 с заголовком "Robot". Создаёт OpenGL контекст, загружает стандартный шрифт и текстуру.

```cpp
    SetTargetFPS(60);
```
Ограничивает частоту кадров до 60 FPS. Без этого цикла будет работать на максимальной скорости.

```cpp
    while (!WindowShouldClose()) {
```
Главный игровой цикл. Продолжается, пока не будет нажат ESC или закрыто окно.

```cpp
        BeginDrawing();
```
Начинает рисование. Все draw-команды должны быть между `BeginDrawing()` и `EndDrawing()`.

```cpp
        ClearBackground((Color){ 50, 50, 50, 255 });
```
Очищает экран тёмно-серым цветом (R=50, G=50, B=50, A=255). Используется compound literal для создания структуры Color.

```cpp
        EndDrawing();
```
Заканчивает рисование, меняет буферы (swap buffers). Вызывает `SwapScreenBuffer()` и `PollInputEvents()`.

```cpp
    }
```
Конец цикла.

```cpp
    CloseWindow();
```
Закрывает окно, выгружает OpenGL контекст и все ресурсы.

```cpp
    return 0;
```
Возвращает 0 — успешное завершение программы.

---

## 6. Типичные паттерны

### 6.1 Игровой цикл

```c
#include "raylib.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Заголовок");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // 1. Обновление логики
        // 2. Рисование
        BeginDrawing();
        ClearBackground(RAYWHITE);
        // ... фигуры, текстуры, текст
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
```

### 6.2 Рисование фигур

```c
BeginDrawing();
ClearBackground(RAYWHITE);

DrawRectangle(50, 50, 200, 100, RED);
DrawCircle(400, 300, 80, BLUE);
DrawLine(0, 0, 800, 600, GREEN);
DrawTriangle(
    (Vector2){100, 400}, (Vector2){200, 400}, (Vector2){150, 300},
    YELLOW
);

EndDrawing();
```

### 6.3 Обработка ввода

```c
// Клавиатура
if (IsKeyPressed(KEY_SPACE)) { /* действие */ }
if (IsKeyDown(KEY_RIGHT)) { player.x += 5.0f; }

// Мышь
Vector2 mouse = GetMousePosition();
if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { /* клик */ }
float wheel = GetMouseWheelMove();

// Геймпад
if (IsGamepadAvailable(0)) {
    float x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
}
```

### 6.4 Загрузка и отрисовка текстур

```c
Texture2D texture = LoadTexture("sprite.png");

BeginDrawing();
ClearBackground(RAYWHITE);
DrawTexture(texture, 100, 100, WHITE);          // оригинальный размер
DrawTextureEx(texture, (Vector2){300, 100}, 45.0f, 2.0f, WHITE); // с поворотом и масштабом
EndDrawing();

UnloadTexture(texture); // обязательная выгрузка
```

### 6.5 Работа с текстом

```c
Font font = LoadFont("arial.ttf");

BeginDrawing();
ClearBackground(RAYWHITE);

DrawText("Hello, World!", 10, 10, 20, BLACK);
DrawTextEx(font, "Custom font!", (Vector2){10, 40}, 24, 2.0f, BLACK);
DrawFPS(10, 70);

EndDrawing();

UnloadFont(font);
```

### 6.6 2D Камера

```c
Camera2D camera = { 0 };
camera.target = (Vector2){ 0, 0 };
camera.offset = (Vector2){ screenWidth/2, screenHeight/2 };
camera.rotation = 0.0f;
camera.zoom = 1.0f;

while (!WindowShouldClose()) {
    // Управление камерой
    if (IsKeyDown(KEY_RIGHT)) camera.target.x += 5;
    if (IsKeyDown(KEY_LEFT))  camera.target.y -= 5;
    float wheel = GetMouseWheelMove();
    if (wheel != 0) camera.zoom += wheel * 0.1f;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);
    DrawRectangle(-100, -100, 200, 200, RED);
    EndMode2D();
    EndDrawing();
}
```

### 6.7 Простой 3D рендеринг

```c
Camera3D camera = { 0 };
camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
camera.fovy = 45.0f;
camera.projection = CAMERA_PERSPECTIVE;

while (!WindowShouldClose()) {
    UpdateCamera(&camera, CAMERA_ORBITAL);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(camera);
    DrawCube((Vector3){0,0,0}, 2.0f, 2.0f, 2.0f, RED);
    DrawCubeWires((Vector3){0,0,0}, 2.0f, 2.0f, 2.0f, MAROON);
    DrawGrid(10, 1.0f);
    EndMode3D();
    DrawFPS(10, 10);
    EndDrawing();
}
```

### 6.8 Аудио

```c
InitAudioDevice();
Sound fx = LoadSound("coin.wav");
Music music = LoadMusicStream("bgm.ogg");
PlayMusicStream(music);

while (!WindowShouldClose()) {
    UpdateMusicStream(music); // обязательно каждый кадр

    if (IsKeyPressed(KEY_SPACE)) PlaySound(fx);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
}

UnloadSound(fx);
UnloadMusicStream(music);
CloseAudioDevice();
```

---

## 7. Устранение проблем

### Ошибки линковки (architecture mismatch)

**Симптомы:**
```
ld: warning: ignoring file .../libraylib.a, building for 'arm64' but trying to link with file built for 'x86_64'
Undefined symbols for architecture arm64
```

**Причина:** Архитектура библиотеки не совпадает с архитектурой компиляции. На Apple Silicon Mac (M1/M2/M3) по умолчанию компилируется под arm64, а библиотека могла быть установлена для x86_64.

**Решение:**
- Переустановите raylib через Homebrew: `brew reinstall raylib`
- Убедитесь, что Homebrew установлен для arm64 (проверка: `which brew` должен быть в `/opt/homebrew/bin/brew`)
- Добавьте в CMakeLists.txt: `set(CMAKE_OSX_ARCHITECTURES "arm64")`

### Ошибки сборки

**Симптомы:**
```
fatal error: 'raylib.h' file not found
```

**Причина:** CMake не может найти заголовочные файлы raylib.

**Решение:**
- Проверьте, установлен ли raylib: `brew list raylib`
- Проверьте `CMAKE_PREFIX_PATH`: убедитесь, что путь правильный (`/opt/homebrew` для Apple Silicon, `/usr/local` для Intel)
- Для ручного указания: `cmake -DCMAKE_PREFIX_PATH=/opt/homebrew ..`

### Как проверить версию raylib

```bash
# Через Homebrew
brew info raylib

# В коде
printf("Raylib version: %s\n", RAYLIB_VERSION);
// или
printf("Raylib %d.%d.%d\n", RAYLIB_VERSION_MAJOR, RAYLIB_VERSION_MINOR, RAYLIB_VERSION_PATCH);
```

### raylib не найден — что делать

**Симптом:** `find_package(raylib REQUIRED)` выдаёт ошибку.

**Пошаговое решение:**

1. Установите raylib через Homebrew:
```bash
brew install raylib
```

2. Проверьте, что файлы CMake существуют:
```bash
ls /opt/homebrew/lib/cmake/raylib/
```
Должен быть `raylib-config.cmake`.

3. Если файла нет — переустановите:
```bash
brew reinstall raylib
```

4. Используйте ручной путь, если автоматический поиск не работает:
```cmake
set(CMAKE_PREFIX_PATH "/opt/homebrew")
set(raylib_INCLUDE_DIRS "/opt/homebrew/include")
set(raylib_LIBRARIES "/opt/homebrew/lib/libraylib.dylib")
```

5. Альтернатива: скопируйте raylib в папку проекта и подключите вручную:
```cmake
add_library(raylib STATIC IMPORTED)
set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/libraylib.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include"
)
```

---

## Математический модуль raymath

### Скалярные функции

#### `Clamp(value, min, max)` — ограничивает значение
#### `Lerp(start, end, amount)` — линейная интерполяция
#### `Normalize(value, start, end)` — нормализация в диапазон
#### `Remap(value, inStart, inEnd, outStart, outEnd)` — пересчёт из одного диапазона в другой
#### `Wrap(value, min, max)` — заворачивание значения
#### `FloatEquals(x, y)` — сравнение float с эпсилоном

### Vector2 (полный список)

`Vector2Zero()`, `Vector2One()`, `Vector2Add(v1,v2)`, `Vector2AddValue(v,add)`, `Vector2Subtract(v1,v2)`, `Vector2SubtractValue(v,sub)`, `Vector2Length(v)`, `Vector2LengthSqr(v)`, `Vector2DotProduct(v1,v2)`, `Vector2Distance(v1,v2)`, `Vector2DistanceSqr(v1,v2)`, `Vector2Angle(v1,v2)`, `Vector2LineAngle(start,end)`, `Vector2Scale(v,scale)`, `Vector2Multiply(v1,v2)`, `Vector2Negate(v)`, `Vector2Divide(v1,v2)`, `Vector2Normalize(v)`, `Vector2Transform(v,mat)`, `Vector2Lerp(v1,v2,amount)`, `Vector2Reflect(v,n)`, `Vector2Min(v1,v2)`, `Vector2Max(v1,v2)`, `Vector2Rotate(v,angle)`, `Vector2MoveTowards(v,target,maxDist)`, `Vector2Invert(v)`, `Vector2Clamp(v,min,max)`, `Vector2ClampValue(v,min,max)`, `Vector2Equals(p,q)`, `Vector2Refract(v,n,r)`

### Vector3 (полный список)

`Vector3Zero()`, `Vector3One()`, `Vector3Add(v1,v2)`, `Vector3AddValue(v,add)`, `Vector3Subtract(v1,v2)`, `Vector3SubtractValue(v,sub)`, `Vector3Scale(v,scalar)`, `Vector3Multiply(v1,v2)`, `Vector3CrossProduct(v1,v2)`, `Vector3Perpendicular(v)`, `Vector3Length(v)`, `Vector3LengthSqr(v)`, `Vector3DotProduct(v1,v2)`, `Vector3Distance(v1,v2)`, `Vector3DistanceSqr(v1,v2)`, `Vector3Angle(v1,v2)`, `Vector3Negate(v)`, `Vector3Divide(v1,v2)`, `Vector3Normalize(v)`, `Vector3Project(v1,v2)`, `Vector3Reject(v1,v2)`, `Vector3OrthoNormalize(v1*,v2*)`, `Vector3Transform(v,mat)`, `Vector3RotateByQuaternion(v,q)`, `Vector3RotateByAxisAngle(v,axis,angle)`, `Vector3MoveTowards(v,target,maxDist)`, `Vector3Lerp(v1,v2,amount)`, `Vector3CubicHermite(v1,t1,v2,t2,amount)`, `Vector3Reflect(v,n)`, `Vector3Min(v1,v2)`, `Vector3Max(v1,v2)`, `Vector3Barycenter(p,a,b,c)`, `Vector3Unproject(src,proj,view)`, `Vector3ToFloatV(v)`, `Vector3Invert(v)`, `Vector3Clamp(v,min,max)`, `Vector3ClampValue(v,min,max)`, `Vector3Equals(p,q)`, `Vector3Refract(v,n,r)`

### Vector4

`Vector4Zero()`, `Vector4One()`, `Vector4Add`, `Vector4AddValue`, `Vector4Subtract`, `Vector4SubtractValue`, `Vector4Length`, `Vector4LengthSqr`, `Vector4DotProduct`, `Vector4Distance`, `Vector4DistanceSqr`, `Vector4Scale`, `Vector4Multiply`, `Vector4Negate`, `Vector4Divide`, `Vector4Normalize`, `Vector4Min`, `Vector4Max`, `Vector4Lerp`, `Vector4MoveTowards`, `Vector4Invert`, `Vector4Equals`

### Matrix

`MatrixDeterminant(mat)`, `MatrixTrace(mat)`, `MatrixTranspose(mat)`, `MatrixInvert(mat)`, `MatrixIdentity()`, `MatrixAdd(left,right)`, `MatrixSubtract(left,right)`, `MatrixMultiply(left,right)`, `MatrixTranslate(x,y,z)`, `MatrixRotate(axis,angle)`, `MatrixRotateX(angle)`, `MatrixRotateY(angle)`, `MatrixRotateZ(angle)`, `MatrixRotateXYZ(angle)`, `MatrixRotateZYX(angle)`, `MatrixScale(x,y,z)`, `MatrixFrustum(l,r,b,t,n,f)`, `MatrixPerspective(fovY,aspect,near,far)`, `MatrixOrtho(l,r,b,t,n,f)`, `MatrixLookAt(eye,target,up)`, `MatrixToFloatV(mat)`, `MatrixDecompose(mat,*translation,*rotation,*scale)`

### Quaternion

`QuaternionAdd(q1,q2)`, `QuaternionAddValue(q,add)`, `QuaternionSubtract`, `QuaternionSubtractValue`, `QuaternionIdentity()`, `QuaternionLength(q)`, `QuaternionNormalize(q)`, `QuaternionInvert(q)`, `QuaternionMultiply(q1,q2)`, `QuaternionScale(q,mul)`, `QuaternionDivide(q1,q2)`, `QuaternionLerp(q1,q2,amount)`, `QuaternionNlerp(q1,q2,amount)`, `QuaternionSlerp(q1,q2,amount)`, `QuaternionCubicHermiteSpline(q1,t1,q2,t2,t)`, `QuaternionFromVector3ToVector3(from,to)`, `QuaternionFromMatrix(mat)`, `QuaternionToMatrix(q)`, `QuaternionFromAxisAngle(axis,angle)`, `QuaternionToAxisAngle(q,*axis,*angle)`, `QuaternionFromEuler(pitch,yaw,roll)`, `QuaternionToEuler(q)`, `QuaternionTransform(q,mat)`, `QuaternionEquals(p,q)`

### C++ операторы (при включении RAYMATH_DISABLE_CPP_OPERATORS)

Для Vector2, Vector3, Vector4, Quaternion, Matrix доступны перегруженные операторы:
`+`, `-`, `*`, `/`, `+=`, `-=`, `*=`, `/=`, `==`, `!=`
Константы: `Vector2Zeros`, `Vector2Ones`, `Vector2UnitX`, `Vector2UnitY` и аналоги для Vector3, Vector4, Quaternion.
