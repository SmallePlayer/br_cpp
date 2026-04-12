# 🤖 br_cpp — Функциональный аналог ROS без классов

> Лёгковесная распределённая система обмена сообщениями (pub/sub), построенная **исключительно на функциях** — без классов в главной архитектуре. Поддерживает **TCP** и **UDP** транспорта. Минимальные зависимости. Только C++17 и POSIX.

---

## 🎯 Зачем

Классический ROS (Robot Operating System) тянет за собой тяжелую объектно-ориентированную инфраструктуру — `rclcpp`, `nodelet`, слой абстракций поверх абстракций. Этот проект предлагает альтернативу:

- **Никаких классов в ядре** — только функции. Прозрачно, отлаживаемо, понятно.
- **TCP + UDP** — выбирай протокол под задачу. TCP для надёжности, UDP для скорости.
- **Минимум зависимостей** — нет rclcpp, нет классов, нет магии. Только POSIX сокеты и C++17.
- **Работает как ROS** — топики, publisher/subscriber, центральный брокер. Знакомая модель, другая реализация.

---

## ⚡ Быстрый старт

### Сборка

```bash
mkdir build && cd build
cmake ..
make
```

### Запуск: Publisher/Subscriber

```bash
# Терминал 1 — запускаем брокер
./broker

# Терминал 2 — подписываемся на топик
./sub

# Терминал 3 — публикуем в топик
./pub
```

### Запуск: Видеопоток

```bash
./broker          # Брокер
./reciv_frame     # Приёмник видео
./send_frame      # Отправитель видео
```

### Запуск: Планирование пути

```bash
./algoritm        # Демо A* и Брезенхема
```

---

## 🏗️ Архитектура

```
br_cpp/
├── CMakeLists.txt               # Конфигурация сборки
├── README.md                    # Документация
├── ARCHITECTURE.md              # Детальная архитектура
│
├── network/                     # 🌐 Сетевое ядро и брокер
│   ├── core/
│   │   ├── broker.cpp           #   Центральный pub/sub брокер
│   │   ├── net.cpp              #   Реализация TCP/UDP функций
│   │   ├── net.h                #   Интерфейс сети (все функции)
│   │   ├── config.hpp           #   HOST, PORT и другие константы
│   │   ├── signals.cpp/.hpp     #   Обработка SIGINT/SIGTERM
│   │   ├── optimization.cpp/.h  #   Оптимизации
│   │   ├── br_time.h            #   Утилиты задержек
│   │   └── time.cpp             #   Временные утилиты
│   ├── pub.cpp                  #   Publisher-клиент
│   ├── sub.cpp                  #   Subscriber-клиент
│   ├── tcp_client.cpp           #   TCP клиент
│   ├── tcp_server.cpp           #   TCP сервер
│   ├── send_frame.cpp           #   Отправка видеокадров
│   └── reciv_frame.cpp          #   Приём видеокадров
│
├── camera/                      # 📷 Работа с камерой (OpenCV)
│   ├── capture.cpp/.h           #   Захват видеопотока
│   ├── frames.cpp/.h            #   Буферизация кадров
│   └── jpeg_work.cpp/.h         #   JPEG кодирование
│
├── logick/                      # 🧠 Алгоритмы планирования пути
│   ├── a_star.cpp/.h            #   Алгоритм A*
│   ├── brezenhem.cpp/.h         #   Алгоритм Брезенхема
│   └── search_path.cpp          #   Комбинированный поиск пути
│
├── src/
│   └── frame_find.cpp           #   ArUco маркеры
│
├── ardu_tcp/                    # 🔌 Arduino клиент (ESP32)
│   └── ardu_tcp.ino             #   WiFi TCP скетч
│
└── py_code/                     # 🐍 Python утилиты
    └── py_tcp_server.py          #   Тестовый TCP сервер
```

---

## 🔌 Сетевая модель: TCP + UDP

Проект поддерживает **два транспортных протокола**, которые выбираются в зависимости от задачи:

### TCP — надёжная доставка
- Гарантированный порядок и доставка сообщений
- Используется для управления, команд, топиков где потеря данных критична
- Функции: `create_tcp_socket()`, `settings_server_socket()`, `connect_server()`

### UDP — максимальная скорость
- Без гарантии доставки, минимальная задержка
- Идеально для видеопотока, телеметрии, сенсорных данных
- Функции: `create_socket()`, `settings_udp_publisher()`, `settings_udp_sub()`

```
┌─────────────┐      TCP       ┌──────────┐      TCP       ┌─────────────┐
│  Publisher  │ ───────────── │  Broker  │ ───────────── │  Subscriber │
│  (pub.cpp)  │  send_data()  │ :8080    │  recv_data()  │  (sub.cpp)  │
└─────────────┘               └──────────┘               └─────────────┘
                                     │
                                     │ UDP
                               ┌──────────┐
                               │ Видеопот │
                               │ / Телемет│
                               └──────────┘
```

---

## 📖 API: Всё на функциях

### Создание Publisher/Subscriber

```cpp
#include "net.h"

// Создать publisher для топика "commands"
int pub_fd = create_publisher("commands");

// Создать subscriber для топика "status"
int sub_fd = create_subscriber("status");
```

### Отправка и приём данных

```cpp
// Шаблонная отправка — любой POD тип
MyStruct data{.x = 10, .y = 20};
send_data(pub_fd, data);

// Шаблонный приём с проверкой статуса
MyStruct received;
RecvStatus status = reciv_data(sub_fd, received);

if (check_disconnect(status, sub_fd) != 0) {
    // Обработка отключения или ошибки
}
```

### UDP: Быстрая отправка

```cpp
// Создать UDP publisher
int udp_fd = create_pub();

// Создать UDP subscriber и привязать
int udp_sub = create_sub();
settings_udp_sub(udp_sub);

// Приём целого числа
int value;
recv_int(udp_sub, value);
```

### Работа с камерой

```cpp
#include "capture.h"
#include "frames.h"

cv::VideoCapture cap(0);
capture_camera(cap);        // Захват в отдельном потоке
cv::Mat frame = get_frame(); // Потокобезопасное получение кадра
```

### Планирование пути

```cpp
#include "a_star.h"
#include "brezenhem.h"

// Стратегия: Брезенхем → A* при препятствии
// 1. Проверяем прямую видимость (Брезенхем)
// 2. Если препятствие — запускаем A*
// 3. Повторяем до достижения цели
```

---

## 🔧 Конфигурация

### Сетевые параметры (`network/core/config.hpp`)

```cpp
constexpr const char* HOST = "127.0.0.1";
constexpr int PORT = 8080;
```

### Переопределение через CMake

```cmake
target_compile_definitions(your_target PRIVATE
    HOST="192.168.1.100"
    PORT=9090
)
```

---

## 📦 Использование как подмодуль

### Подключение

```bash
cd your_project
git submodule add https://github.com/your/br_cpp.git third_party/br_cpp
```

### В вашем CMakeLists.txt

```cmake
add_subdirectory(third_party/br_cpp)

target_link_libraries(your_target
    br_cpp::network      # сетевое ядро + брокер
    br_cpp::camera       # работа с камерой
    br_cpp::logick       # алгоритмы пути
)
```

---

## 🧩 Исполняемые файлы

| Цель | Источники | Описание |
|------|-----------|----------|
| `broker` | broker, net, time, signals | Центральный pub/sub брокер |
| `pub` | pub, net, time, signals | Publisher-клиент |
| `sub` | sub, net, time, signals | Subscriber-клиент |
| `tcp_server` | tcp_server, net, time, signals | TCP сервер |
| `tcp_client` | tcp_client, net, time, signals | TCP клиент |
| `send_frame` | send_frame, net, frames, jpeg_work, signals | Отправка видео по сети |
| `reciv_frame` | reciv_frame, net, frames, jpeg_work, signals | Приём видео по сети |
| `capture` | capture, frames, jpeg_work | Захват камеры |
| `algoritm` | search_path, a_star, brezenhem | Демо планирования пути |

---

## 🧠 Философия: Почему без классов

| Классы | Функции |
|--------|---------|
| Скрытое состояние (инкапсуляция → сюрпризы) | Явная передача контекста через аргументы |
| Сложная иерархия наследования | Плоская структура, понятный call-graph |
| Неявные зависимости (this, virtual) | Явные зависимости (параметры функций) |
| Тяжело тестировать изолированно | Легко мокировать и подменять |
| ROS: `rclcpp::Node` → `NodeOptions` → `Publisher<T>` → callback hell | br_cpp: `create_publisher("topic")` → `send_data(fd, data)` → done |

**Ключевой принцип**: каждый файл — набор функций. Каждый сокет — просто `int`. Каждый топик — строка. Никакой магии.

---

## 🔗 Интеграция с Arduino

Проект включает готовый скетч для ESP32, который подключается к брокеру по WiFi:

```cpp
// ardu_tcp/ardu_tcp.ino
// 1. Подключается к WiFi
// 2. Устанавливает TCP соединение с брокером
// 3. Отправляет инкрементальный счётчик в топик
```

**Запуск:**
1. Загрузить `ardu_tcp.ino` на ESP32
2. Запустить `./broker` на хост-машине
3. Arduino автоматически подключается и публикует данные

---

## 🔬 Потоки данных

### Pub/Sub (TCP)
```
Publisher ──send_data()──► Broker ──recv_data()──► Subscriber
                                │
                          Arduino (WiFi TCP)
```

### Видеопоток (UDP + JPEG)
```
Камера ──capture()──► JPEG сжатие ──send_frame()──► Сеть (UDP)
                                                          │
Receiver ◄──get_frame()◄── Декодирование ◄──recv_frame()──┘
```

### Планирование пути
```
Карта препятствий ──► Брезенхем (быстрая проверка) ──► Прямой путь?
                                                     │
                                                     └──► A* (обход препятствий) ──► Траектория
```

---

## 📋 Зависимости

| Зависимость | Версия | Назначение |
|-------------|--------|------------|
| C++17 | GCC 9+ | Основной язык |
| CMake | 3.10+ | Сборка |
| POSIX sockets | — | TCP/UDP сеть |
| pthreads | — | Многопоточность |
| OpenCV | 4.x | Камера и ArUco |
| Arduino SDK | — | Для ESP32 скетча |

---

## 📄 Лицензия

MIT
```

---

Покажите текущую структуру ваших файлов — уточню пути и компоненты:

```bash
find /Users/pes/code/br_cpp -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" \) | head -20