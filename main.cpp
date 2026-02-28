#include <iostream>
#include <vector>
#include "brezenhem.h"




int main() {
    std::vector<std::vector<int>> map_matrix(100, std::vector<int>(100, 0));

    int x1{10};
    int x2{20};
    int y1{10};
    int y2{20};

    for(int i{x1}; i <= x2; ++i){
        for(int j{y1}; j <= y2; ++j){
            map_matrix[i][j] = 1;
        }
    }
    

    Result_brezenhem answer = algoritmv_brezenhem(map_matrix, 0,0,22,23);
    std::cout << "X_end: " << answer.x_end << " Y_end: " << answer.y_end << " found: " << answer.found << std::endl;
    return 0;
}