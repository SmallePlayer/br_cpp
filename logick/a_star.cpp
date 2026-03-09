#include "a_star.h"
#include <iostream>
#include <limits>
#include <cmath>

//эвристика (манхетоновское)
int heuristik(int x_self, int y_self, int x_target, int y_target){
    int variable = abs(x_target - x_self) + abs(y_target -  y_self);
    return variable;
}

// сам алгоритм А
Algoritm_Brezenhem algoritm_A(const std::vector<std::vector<int>>& map_matrix, int x_self, int y_self, int x_target, int y_target){
    Algoritm_Brezenhem result;
    // текущие координаты
    int current_x{x_self};
    int current_y{y_self};

    
    
    // то в какие стороны смотреть и ходить
    std::vector<std::pair<int, int>> dir = {{-1, 0}, {0, +1}, {1, +0}, {0, -1}};

    // цикл пока не дойдет до конца(current_x != x_target || current_y != y_target) (можно сделать по одному шагу)
    for(uint8_t step = 0; step <= 1; ++step){
        // иницализация лучшей евристики из точки
        int best_dx{0};
        int best_dy{0};
        //максимальное значние для сравнение с евристикой
        double min_heytistik = std::numeric_limits<double>::max();

        // цикл проверки по 4 стороном и проверка могу не могу туда зайти
        for(auto [dx, dy] : dir){
            // определяем следующую ячейку для проверки
            int new_x = current_x + dx;
            int new_y = current_y + dy;

            // проверка на выход за границы массива
            if(new_x < 0 || new_x >= map_matrix.size() || new_y < 0 || new_y >= map_matrix[0].size()) continue;
            // std::cout << "current: " << map_matrix[current_x][current_y] << std::endl;
            //std::cout << "new" << map_matrix[new_x][new_y] << std::endl;
            // std::cout << "current_x: " << current_x << " curretn_y: " << current_y << std::endl;
            //std::cout << "new_x: " << new_x << " new_y: " << new_y << std::endl;
            int heyrist = heuristik(new_x, new_y, x_target, y_target);
            //std::cout << "heyristik: " << heyrist << std::endl;
            
            //проверка на то есть ли препядствие или нет (используется для отладки)
            // if (map_matrix[new_x][new_y] == 5){
            //     std::cout << "obscale" << std::endl;
            // }

            // определение минимальной эврисики
            if(heyrist < min_heytistik && map_matrix[new_x][new_y] != 5){
                min_heytistik = heyrist;
                best_dx = dx;
                best_dy = dy;
                // std::cout << "best_dx: " << best_dx << " best_dy: " << best_dy << std::endl;
                
            }

            
        }
        // делаем шаг в сторону лучшей эвристики
        current_x += best_dx;
        current_y += best_dy;
        // std::cout << "________________" << std::endl;
    }
    if (current_x == x_target && current_y == y_target){
        result.found = true;
    }
    result.x_end = current_x;
    result.y_end = current_y;
    return result;
}