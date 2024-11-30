//
// Created by bl4an on 2024-11-30.
//

#include "./generate_random_data.h"

float *generate_random_data(const int dim, const int max_elements) {
    std::mt19937 rng;
    rng.seed(47);
    std::uniform_real_distribution<> distrib_real;

    float* data = new float[dim * max_elements];
    for (int i = 0; i < dim * max_elements; i++) {
        data[i] = distrib_real(rng);
    }

    return data;
}

float* generate_random_cluster_data(const int dim, const int max_elements) {
    // Constants for clusters
    const int num_clusters = 8; // Number of clusters
    const int elements_per_cluster = max_elements / num_clusters; // Elements in each cluster
    const float cluster_offset = 100.0f; // Distance between cluster centroids

    // Random number generator
    std::mt19937 rng(47);
    std::uniform_real_distribution<> distrib_real(-5.0, 5.0); // Range of random values within a cluster

    // Allocate memory for the data
    float* data = new float[dim * max_elements];

    // Generate clusters
    for (int cluster_idx = 0; cluster_idx < num_clusters; ++cluster_idx) {
        // Define the centroid for the current cluster
        std::vector<float> centroid(dim);
        for (int d = 0; d < dim; ++d) {
            centroid[d] = cluster_idx * cluster_offset; // Spread centroids apart
        }

        // Generate data for the current cluster
        for (int i = 0; i < elements_per_cluster; ++i) {
            int index = (cluster_idx * elements_per_cluster + i) * dim; // Starting index for this vector
            for (int d = 0; d < dim; ++d) {
                data[index + d] = centroid[d] + distrib_real(rng); // Add random offset to the centroid
            }
        }
    }

    return data;
}

void shuffle_data(float* data, const int dim, const int max_elements) {
    // Create a vector of indices for shuffling
    std::vector<int> indices(max_elements);
    for (int i = 0; i < max_elements; ++i) {
        indices[i] = i;
    }

    // Random number generator
    std::mt19937 rng(47); // Seed for reproducibility
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