#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <chrono>

#include "brezenhem.h"
#include "a_star.h"





int main() {
    auto start = std::chrono::high_resolution_clock::now();

    //me xy
    int x_self{3};
    int y_self{3};

    //target xy
    int x_target{400};
    int y_target{200};

    // текущие координаты
    int current_x{0};
    int current_y{0};

    //matrix main
    std::vector<std::vector<int>> map_matrix(640, std::vector<int>(480, 0));

    // моя точка на карте 
    map_matrix[x_self][y_self] = 1;

    //matrix obscale
    int x1{580};
    int x2{600};
    int y1{440};
    int y2{460};

    //fill main matrix obscale
    for(int i{x1}; i <= x2; ++i){
        for(int j{y1}; j <= y2; ++j){
            map_matrix[i][j] = 5;
        }
    }

    bool finish{false};

    while(finish == false){
        //call algoritm brezenhema
        Algoritm_Brezenhem answer = algoritm_brezenhem(map_matrix, current_x, current_y, x_target, y_target);
        current_x = answer.x_end;
        current_y = answer.y_end;
        finish = answer.found;
        //std::cout << "X_end: " << answer.x_end << " Y_end: " << answer.y_end << " found: " << answer.found << std::endl;
        if(answer.found == false){
            Algoritm_Brezenhem answer_A = algoritm_A(map_matrix, current_x, current_y, x_target, y_target);
            current_x = answer_A.x_end;
            current_y = answer_A.y_end;
            finish = answer_A.found;
            //std::cout << "A* exit: X=" << answer_A.x_end << " Y=" << answer_A.y_end << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - start);

    std::cout << "Время выполнения: " << duration.count() << " секунд" << std::endl;

    // std::cout << "---------------" << std::endl;
    std::cout << "current_X: " << current_x << std::endl;
    std::cout << "current_Y: " << current_y << std::endl;
    std::cout << "FINISH" << std::endl;
    // std::cout << "---------------" << std::endl;

    
    
    return 0;
}