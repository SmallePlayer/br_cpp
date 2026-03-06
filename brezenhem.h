#ifndef BREZENHEM_H
#define BREZENHEM_H

#include <iostream>
#include <vector>


struct Algoritm_Brezenhem {
    std::vector<std::pair<int, int>> path;
    int x_end;
    int y_end;
    bool found;
};

Algoritm_Brezenhem algoritm_brezenhem(const std::vector<std::vector<int>>& map_matrix, int x_self, int y_self, int x_target, int y_target);

#endif