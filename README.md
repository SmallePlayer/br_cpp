# br_cpp

C++ проект: алгоритмы пути, многопоточный захват камеры, передача кадров по TCP/UDP.

## Структура проекта

```
br_cpp/
├── main.cpp                  # алгоритмы (Брезенхем, A*)
├── camera/
│   ├── capture.cpp           # запуск многопоточного захвата камеры
│   ├── frames.cpp            # capture_camera() и get_frame() — фоновый поток
│   └── jpeg_work.cpp         # jpeg_compress() и jpeg_decompress()
├── network/
│   ├── send_frame.cpp        # TCP клиент: захват камеры → отправка JPEG кадров
│   ├── reciv_frame.cpp       # TCP сервер: приём JPEG кадров → отображение
│   ├── udp_client.cpp        # UDP клиент: приём кадров
│   ├── udp_server.cpp        # UDP сервер: отправка кадров
│   ├── tcp_client.cpp        # TCP клиент (структуры)
│   └── tcp_server.cpp        # TCP сервер (структуры)
├── logick/
│   ├── brezenhem.cpp         # алгоритм Брезенхема
│   ├── a_star.cpp            # алгоритм A*
│   └── search_path.cpp       # поиск пути
├── include/                  # заголовочные файлы
└── CMakeLists.txt
```

## Сборка

```bash
mkdir build && cd build
cmake ..
make
```

## Запуск передачи кадров по TCP

```bash
# Терминал 1 — сервер (получатель)
./reciv_frame

# Терминал 2 — клиент (отправитель, запускает камеру)
./send_frame
```

## API

### Структура Result_brezenhem

```cpp
struct Result_brezenhem {
    std::vector<std::pair<int, int>> path;  // путь от стартовой до целевой точки
    int x_end;                               // финальная координата X
    int y_end;                               // финальная координата Y
    bool found;                              // была ли найдена цель
};
```

### Функция algoritmv_brezenhem

```cpp
Result_brezenhem algoritmv_brezenhem(
    const std::vector<std::vector<int>>& map_matrix,
    int x_self, int y_self,
    int x_target, int y_target
);
```

**Параметры:**
- `map_matrix` — 2D матрица препятствий (1 = препятствие, 0 = свободно)
- `x_self, y_self` — начальные координаты
- `x_target, y_target` — целевые координаты

**Возвращает:** структуру `Result_brezenhem` с найденным путем

## Пример

```cpp
std::vector<std::vector<int>> map_matrix(100, std::vector<int>(100, 0));
Result_brezenhem result = algoritmv_brezenhem(map_matrix, 0, 0, 50, 50);

if (result.found) {
    std::cout << "Путь найден!" << std::endl;
    for (auto [x, y] : result.path) {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
}
```

## Требования

- C++17 или выше
- Компилятор: clang++ или g++
