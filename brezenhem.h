#ifndef BREZENHEM_H
#define BREZENHEM_H

#include <iostream>
#include <vector>


struct Result_brezenhem {
    std::vector<std::pair<int, int>> path;
    int x_end;
    int y_end;
    bool found;
};

Result_brezenhem algoritmv_brezenhem(const std::vector<std::vector<int>>& map_matrix, int x_self, int y_self, int x_target, int y_target);

#endif