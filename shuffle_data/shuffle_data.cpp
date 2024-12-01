//
// Created by bl4an on 2024-12-01.
//

#include "./shuffle_data.h"

void shuffle_data(float* data, const int dim, const int max_elements, const int seed) {
    // Create a vector of indices for shuffling
    std::vector<int> indices(max_elements);
    for (int i = 0; i < max_elements; ++i) {
        indices[i] = i;
    }

    // Random number generator
    std::mt19937 rng(seed); // Seed for reproducibility
    std::shuffle(indices.begin(), indices.end(), rng);

    // Create a temporary buffer to hold the shuffled data
    float* temp_data = new float[dim * max_elements];

    // Shuffle the data based on the shuffled indices
    for (int i = 0; i < max_elements; ++i) {
        int src_idx = indices[i] * dim;
        int dest_idx = i * dim;
        for (int d = 0; d < dim; ++d) {
            temp_data[dest_idx + d] = data[src_idx + d];
        }
    }

    // Copy shuffled data back to the original data array
    for (int i = 0; i < dim * max_elements; ++i) {
        data[i] = temp_data[i];
    }

    // Free the temporary buffer
    delete[] temp_data;
}