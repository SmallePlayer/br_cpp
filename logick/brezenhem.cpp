#include "brezenhem.h"
#include <iostream>

Algoritm_Brezenhem algoritm_brezenhem(const std::vector<std::vector<int>>& map_matrix, int x_self, int y_self, int x_target, int y_target){
    Algoritm_Brezenhem result;
    int prev_x{x_self};
    int prev_y{y_self};
    while(true){
        double rot_koff = static_cast<double>(y_target - y_self) / static_cast<double>(x_target - x_self);

        if(map_matrix[x_self][y_self] == 5){
            result.found = false;
            result.x_end = prev_x;  // позиция ДО препятствия
            result.y_end = prev_y;
            return result;
        }
        if (x_target <= x_self && y_target <= y_self){
            result.found = true;
            result.x_end = x_self;
            result.y_end = y_self;
            return result;
        }

        prev_x = x_self;  // сохраняем текущую (безопасную) позицию
        prev_y = y_self;

        if (x_self < (int)map_matrix.size() - 1){
            x_self += 1;
        }
        if(rot_koff >= 0.5){
            y_self += 1;
        }
    }
}
