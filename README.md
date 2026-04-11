# br_cpp

C++ проект с ROS-нодами для работы с сетевыми компонентами.

## Архитектура проекта

```
br_cpp/
├── CMakeLists.txt              # Основная конфигурация сборки
├── README.md                   # Документация
│
├── include/                    # Публичные заголовки (API)
│   ├── config.hpp              # Глобальные константы (HOST, PORT)
│   └── ros_node.hpp            # Интерфейс ROS-ноды (без rclcpp)
│   └── impl/                   # Приватные заголовки
│       └── ros_node_impl.hpp   # Pimpl-заголовок
│
├── src/                        # Исходники реализаций
│   ├── main.cpp                # Точка входа
│   ├── ros_node.cpp            # Форвардинг реализации
│   ├── ros_node_impl.cpp       # Реальная реализация ROS
│   └── signals.cpp             # Обработка сигналов ОС
│
├── network/                    # Сетевые компоненты
│   ├── net.cpp                 # Сетевые операции
│   ├── net.hpp                 # Интерфейс сетевых функций
│   ├── broker.cpp              # Логика брокера сообщений
│   └── broker.hpp              # Интерфейс брокера
│
└── build/                      # Директория сборки (gitignore)
```

## Быстрый старт

### Сборка

```bash
mkdir build && cd build
cmake ..
make
```

### Запуск

```bash
./broker
./send_frame
```

## Использование в другом проекте

### 1. Подключить как подмодуль

```bash
cd your_project
git submodule add https://github.com/your/br_cpp.git third_party/br_cpp
```

### 2. В вашем CMakeLists.txt

```cmake
add_subdirectory(third_party/br_cpp)

# Вариант A: использовать библиотеку
target_link_libraries(your_target 
    br_cpp::ros_node      # интерфейс ROS
    br_cpp::network       # сетевые компоненты
)

# Вариант B: использовать заголовки
target_include_directories(your_target PRIVATE
    third_party/br_cpp/include
)
```

### 3. В вашем коде

```cpp
// Только один заголовок — никаких rclcpp!
#include "br_cpp/ros_node.hpp"

int main() {
    mynode::RosNode node("my_node");
    
    node.subscribe("commands", [](const std::string& data) {
        std::cout << "Got: " << data << "\n";
    });
    
    node.publish("status", "ready");
    node.spin();
}
```

## Компоненты

| Компонент | Назначение | Зависимости |
|-----------|-----------|-------------|
| `RosNode` | Упрощённый интерфейс ROS | rclcpp (скрыта) |
| `SignalManager` | Обработка SIGINT/SIGTERM | `<csignal>` |
| `Network` | TCP/UDP операции | POSIX sockets |
| `Broker` | Маршрутизация сообщений | RosNode + Network |

## Конфигурация

Переопределите в `include/config.hpp` или через `-D` в cmake:

| Параметр | По умолчанию | Описание |
|----------|-------------|----------|
| `HOST` | `"127.0.0.1"` | Адрес сервера |
| `PORT` | `8080` | Порт сервера |
| `ROS_DOMAIN_ID` | `0` | ID домена ROS 2 |

```cmake
# Переопределение в CMake
target_compile_definitions(your_target PRIVATE
    HOST="192.168.1.100"
    PORT=9090
)
```

## Зависимости

- C++17
- ROS 2 Humble/Iron (для ROS-компонентов)
- CMake 3.8+

## Лицензия

MIT
```

---

Покажите текущую структуру ваших файлов — уточню пути и компоненты:

```bash
find /Users/pes/code/br_cpp -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" \) | head -20