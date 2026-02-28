# Bresenham Algorithm Implementation

Реализация алгоритма линии Брезенхема на C++.

## Описание

Проект содержит реализацию алгоритма Брезенхема для рисования линий с целочисленными координатами. Алгоритм находит оптимальный путь от стартовой точки к целевой, минимизируя количество точек.

## Структура проекта

```
br_cpp/
├── main.cpp              # основная программа
├── brezenhem.h           # заголовочный файл с объявлениями
├── brezenhem.cpp         # реализация алгоритма
├── README.md             # этот файл
└── .gitignore            # игнорирование файлов Git
```

## Использование

### Компиляция

```bash
clang++ -o main main.cpp brezenhem.cpp
```

### Запуск

```bash
./main
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
