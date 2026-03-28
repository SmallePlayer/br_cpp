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

## Оптимизация захвата камеры

- Включите MJPEG/Hardware-compressed режим на камере: `cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G'))`. Это даёт значительное снижение загрузки CPU и USB-канала, так как камера отдаёт уже сжатые JPEG-кадры.
- Запрашивайте подходящее разрешение и FPS: `cap.set(cv::CAP_PROP_FRAME_WIDTH, W)`, `cap.set(cv::CAP_PROP_FRAME_HEIGHT, H)`, `cap.set(cv::CAP_PROP_FPS, F)`. Меньшее разрешение обычно позволяет достичь более высокого FPS.
- Избегайте лишних копий `cv::Mat` в горячем пути. Используйте `std::shared_ptr<cv::Mat>` для обмена кадром между потоками вместо `clone()`.
- Сжатие JPEG: используйте `libjpeg-turbo` или снижайте качество (например, `quality=60`) чтобы уменьшить нагрузку на CPU при кодировании и уменьшить объём передаваемых данных.
- При передаче по TCP отключайте Nagle и увеличивайте буферы сокета: `setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, ...)`, `setsockopt(sock, SOL_SOCKET, SO_SNDBUF, ...)` и `SO_RCVBUF` на приёмной стороне.
- Проверяйте реальные свойства камеры программно: `cap.get(cv::CAP_PROP_FPS)` и `cap.get(cv::CAP_PROP_FOURCC)` — это покажет, какие режимы реально поддерживаются.
- Если всё ещё упёрлись в аппаратные ограничения, рассмотрите камеру с поддержкой более высоких FPS и/или подключение по USB3.

## Последние изменения

### Рефакторинг сетевого кода (v1.1)
- **Удалено:** `settings_socket.h` и `settings_socket.cpp` (устаревшие)
- **Добавлено:** `net.h` и `net.cpp` с универсальными шаблонными функциями
- **Улучшено:** `tcp_client.cpp` и `tcp_server.cpp` теперь используют `send_data()` и `reciv_data()`
- **CMake:** Обновлены зависимости для использования `net.cpp`

### Новые компоненты
- `py_code/py_tcp_server.py` - Python TCP сервер для тестирования
- `src/frame_find.cpp` - дополнительная обработка кадров
- `include/topic_server.h` - заголовочный файл для серверной тематики
