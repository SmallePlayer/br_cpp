#include "brezenhem.h"
#include <iostream>

Algoritm_Brezenhem algoritm_brezenhem(const std::vector<std::vector<int>>& map_matrix, int x_self, int y_self, int x_target, int y_target){
    Algoritm_Brezenhem result;
    while(true){
        //result.path.push_back({x_self, y_self});
        double rot_koff = static_cast<double>(y_target - y_self) / static_cast<double>(x_target - x_self);
        // std::cout <<"Rot_koff: "<< rot_koff << std::endl;
        
        int old_x_self{x_self};
        int old_y_self{y_self};
        if(map_matrix[x_self][y_self] == 5){
            // std::cout << "Pizda" << std::endl;
            result.found = false;
            result.x_end = old_x_self;
            result.y_end = old_y_self;
            return result;
        }
        if (x_target <= x_self && y_target <= y_self){
            // std::cout << "y:" << y_self << "  x:" << x_self << std::endl;
            // std::cout << "Target find. X: " << x_self << " Y: " << y_self << std::endl;
            result.found = true;
            result.x_end = old_x_self;
            result.y_end = old_y_self;
            return result;
        }
        if (x_self < map_matrix.size() - 1){
            x_self += 1;
        }
        if(rot_koff >= 0.5){
            y_self += 1;
        }
        
        // std::cout << "y:" << y_self << "  x:" << x_self << std::endl;
    }
}
