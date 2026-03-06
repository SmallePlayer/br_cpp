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
    int x_target{99};
    int y_target{78};

    // текущие координаты
    int current_x{0};
    int current_y{0};

    //matrix main
    std::vector<std::vector<int>> map_matrix(100, std::vector<int>(100, 0));

    // моя точка на карте 
    map_matrix[x_self][y_self] = 1;

    //matrix obscale
    int x1{0};
    int x2{5};
    int y1{5};
    int y2{10};

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
        // std::cout << "X_end: " << answer.x_end << " Y_end: " << answer.y_end << " found: " << answer.found << std::endl;
        if(answer.found == false){
            Algoritm_Brezenhem answer_A = algoritm_A(map_matrix, current_x, current_y, x_target, y_target);
            current_x = answer_A.x_end;
            current_y = answer_A.y_end;
            finish = answer_A.found;
        }
    }
    // std::cout << "---------------" << std::endl;
    // std::cout << "current_X: " << current_x << std::endl;
    // std::cout << "current_Y: " << current_y << std::endl;
    // std::cout << "FINISH" << std::endl;
    // std::cout << "---------------" << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - start);

    std::cout << "Время выполнения A*: " << duration.count() << " секунд" << std::endl;
    
    return 0;
}