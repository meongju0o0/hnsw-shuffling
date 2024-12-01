//
// Created by bl4an on 2024-11-30.
//

#ifndef GENERATE_RANDOM_DATA_H
#define GENERATE_RANDOM_DATA_H

#include <numeric>
#include <random>
#include <cmath>
#include <vector>
#include <algorithm>

float *generate_random_data(int dim, int max_elements, int seed);
float *generate_random_cluster_data(int dim, int max_elements, int seed);

#endif //GENERATE_RANDOM_DATA_H
