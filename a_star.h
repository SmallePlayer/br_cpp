#ifndef A_STAR_H
#define A_STAR_H

#include <iostream>
#include <vector>
#include "brezenhem.h"


Algoritm_Brezenhem algoritm_A(const std::vector<std::vector<int>>& map_matrix, int x_self, int y_self, int x_target, int y_target);

#endif