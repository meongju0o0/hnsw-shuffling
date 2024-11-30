//
// Created by bl4an on 2024-11-30.
//

#ifndef GENERATE_RANDOM_DATA_H
#define GENERATE_RANDOM_DATA_H

#include "../hnswlib/hnswlib.h"
#include <cmath>
#include <vector>
#include <algorithm>

float *generate_random_data(int dim, int max_elements);
float *generate_random_cluster_data(int dim, int max_elements);
void shuffle_data(float* data, int dim, int max_elements);

#endif //GENERATE_RANDOM_DATA_H