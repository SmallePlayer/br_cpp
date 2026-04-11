# Архитектура проекта br_cpp

## Общее описание

Проект `br_cpp` представляет собой распределенную систему для работы с компьютерным зрением, сетевыми коммуникациями и алгоритмами планирования пути. Система реализует собственный pub/sub брокер сообщений (аналог ROS), работу с камерой, алгоритмы поиска пути (A*, Брезенхем) и интеграцию с Arduino.

## Директория проекта

```
br_cpp/
├── CMakeLists.txt          # Конфигурация сборки CMake
├── README.md               # Базовая документация
├── ARCHITECTURE.md         # Этот файл
│
├── ardu_tcp/               # Arduino-клиент (WiFi TCP)
│   └── ardu_tcp.ino        # Скетч для ESP32/Arduino с WiFi
│
├── camera/                 # Модуль работы с камерой (OpenCV)
│   ├── capture.cpp         # Захват видеопотока
│   ├── capture.h
│   ├── frames.cpp          # Работа с кадрами
│   ├── frames.h
│   ├── jpeg_work.cpp       # JPEG кодирование/декодирование
│   └── jpeg_work.h
│
├── logick/                 # Алгоритмы планирования пути
│   ├── a_star.cpp          # Реализация A*
│   ├── a_star.h
│   ├── brezenhem.cpp       # Алгоритм Брезенхема
│   ├── brezenhem.h
│   └── search_path.cpp     # Демо поиска пути
│
├── network/                # Сетевой стек и брокер сообщений
│   ├── broker.cpp          # Pub/Sub брокер (центральный сервер)
│   ├── br_time.h           # Утилиты задержек
│   ├── config.hpp          # Глобальные константы (HOST, PORT)
│   ├── net.cpp             # Реализация сетевых функций
│   ├── net.h               # Интерфейс сетевых операций
│   ├── pub.cpp             # Publisher-клиент
│   ├── reciv_frame.cpp     # Прием видеокадров
│   ├── send_frame.cpp      # Отправка видеокадров
│   ├── signals.cpp         # Обработка сигналов ОС
│   ├── signals.hpp
│   ├── sub.cpp             # Subscriber-клиент
│   ├── tcp_client.cpp      # TCP клиент
│   ├── tcp_server.cpp      # TCP сервер
│   └── time.cpp            # Утилиты времени
│
├── py_code/                # Python-компоненты
│   ├── py_tcp_server.py    # Тестовый Python TCP сервер
│   └── venv/               # Python окружение
│
└── src/                    # Дополнительные исходники
    └── frame_find.cpp      # Работа с ArUco маркерами
```

## Компоненты системы

### 1. Сетевой модуль (`network/`)

#### Брокер сообщений (`broker.cpp`)
- **Назначение**: Центральный сервер pub/sub архитектуры
- **Функции**:
  - Принимает подключения от publishers и subscribers
  - Маршрутизация сообщений по топикам
  - Управление пулами клиентов в многопоточном режиме
- **Протокол**: TCP + бинарный обмен структурами `ClientHello`

```cpp
struct ClientHello {
    std::string role;    // "pub" или "sub"
    std::string topik;   // имя топика
};
```

#### Сетевой интерфейс (`net.h/cpp`)
- **Функции**:
  - `create_socket()` - создание сокета
  - `settings_server_socket()` - настройка сервера
  - `settings_client_socket()` - настройка клиента
  - `create_publisher()` / `create_subscriber()` - фабрики подключений
  - Шаблонные функции `send_data<T>()` / `reciv_data<T>()` - типобезопасный обмен

#### Клиенты
| Файл | Назначение |
|------|------------|
| `pub.cpp` | Отправляет данные в топик |
| `sub.cpp` | Получает данные из топика |
| `tcp_client.cpp` | Базовый TCP клиент |
| `tcp_server.cpp` | Базовый TCP сервер |
| `send_frame.cpp` | Отправка видеокадров по сети |
| `reciv_frame.cpp` | Прием видеокадров по сети |

### 2. Модуль камеры (`camera/`)

| Компонент | Описание |
|-----------|----------|
| `capture.cpp` | Захват видеопотока с камеры через OpenCV |
| `frames.cpp` | Буферизация и синхронизация кадров (mutex) |
| `jpeg_work.cpp` | Сжатие/распаковка JPEG для передачи по сети |

**API:**
```cpp
void capture_camera(cv::VideoCapture& cap);
cv::Mat get_frame();
```

### 3. Модуль алгоритмов (`logick/`)

#### Алгоритм A* (`a_star.cpp`)
- Поиск пути на матрице с препятствиями
- Оптимальный для длинных дистанций

#### Алгоритм Брезенхема (`brezenhem.cpp`)
- Прямая видимость (line-of-sight)
- Быстрая проверка достижимости

#### Комбинированный поиск (`search_path.cpp`)
```cpp
// Стратегия:
// 1. Пробуем Брезенхема (прямой путь)
// 2. Если препятствие - запускаем A*
// 3. Повторяем до достижения цели
```

### 4. Arduino модуль (`ardu_tcp/`)

- **Платформа**: ESP32/Arduino с WiFi
- **Функции**:
  - Подключение к WiFi
  - TCP-клиент к брокеру
  - Отправка инкрементального счетчика

### 5. Python модуль (`py_code/`)

- `py_tcp_server.py` - тестовый сервер для проверки протокола

### 6. ArUco модуль (`src/frame_find.cpp`)

- Генерация ArUco маркеров
- Детекция маркеров
- Интеграция с OpenCV ArUco модулем

## Потоки данных

### Pub/Sub архитектура
```
┌─────────────┐     TCP      ┌──────────┐     TCP      ┌─────────────┐
│  Publisher  │ ───────────> │  Broker  │ <─────────── │  Subscriber │
│  (pub.cpp)  │  send(data)  │:8080     │  recv(data)  │  (sub.cpp)  │
└─────────────┘              └──────────┘              └─────────────┘
                                    ^
                                    | TCP
                              ┌──────────┐
                              │  Arduino │
                              │(ardu_tcp)│
                              └──────────┘
```

### Видеопоток
```
┌─────────────┐     JPEG     ┌──────────┐
│   Camera    │ ───────────> │ Network  │
│(capture.cpp)│  compressed  │(send_*)   │
└─────────────┘              └──────────┘
```

### Планирование пути
```
┌─────────────┐    Map       ┌──────────┐    Path      ┌──────────┐
│   Obstacle  │ ───────────> │ A* /     │ ───────────> │ Commands │
│    Map      │   matrix     │ Brezenhem│ waypoints    │  output  │
└─────────────┘              └──────────┘              └──────────┘
```

## Конфигурация

### Сетевые параметры (`network/config.hpp`)
```cpp
constexpr const char* HOST = "127.0.0.1";
constexpr int PORT = 8080;
```

### Исполняемые файлы

| Цель CMake | Исходные файлы | Описание |
|------------|----------------|----------|
| `broker` | `broker.cpp, net.cpp, time.cpp, signals.cpp` | Центральный брокер |
| `pub` | `pub.cpp, net.cpp, time.cpp, signals.cpp` | Publisher клиент |
| `sub` | `sub.cpp, net.cpp, time.cpp, signals.cpp` | Subscriber клиент |
| `algoritm` | `search_path.cpp, brezenhem.cpp, a_star.cpp` | Демо планирования |
| `tcp_server` | `tcp_server.cpp, net.cpp, time.cpp, signals.cpp` | TCP сервер |
| `tcp_client` | `tcp_client.cpp, net.cpp, time.cpp, signals.cpp` | TCP клиент |
| `capture` | `capture.cpp, frames.cpp, jpeg_work.cpp` | Захват камеры |
| `send_frame` | `send_frame.cpp, net.cpp, frames.cpp, jpeg_work.cpp, signals.cpp` | Отправка видео |
| `reciv_frame` | `reciv_frame.cpp, net.cpp, frames.cpp, jpeg_work.cpp, signals.cpp` | Прием видео |

## Зависимости

- **C++17**
- **OpenCV 4.x** - компьютерное зрение
- **CMake 3.10+** - сборка
- **POSIX sockets** - сеть (Linux/Unix)
- **Pthreads** - многопоточность
- **Arduino SDK + WiFi.h** - для ESP32

## Паттерны проектирования

1. **Pub/Sub** - брокер сообщений
2. **RAII** - управление сокетами
3. **Pimpl** - скрытая реализация (в README)
4. **Шаблоны** - типобезопасный обмен данными
5. **Producer-Consumer** - буфер кадров камеры
6. **Strategy** - выбор алгоритма планирования (A* vs Брезенхем)

## Сценарии использования

### 1. Тестирование pub/sub
```bash
./broker          # Терминал 1
./sub             # Терминал 2
./pub             # Терминал 3
```

### 2. Видеотрансляция
```bash
./broker          # Терминал 1
./reciv_frame     # Терминал 2
./send_frame      # Терминал 3
```

### 3. Планирование пути
```bash
./algoritm        # Демо алгоритмов
```

### 4. IoT интеграция
- Загрузить `ardu_tcp.ino` на ESP32
- Запустить `./broker`
- Arduino автоматически подключается и отправляет данные
```