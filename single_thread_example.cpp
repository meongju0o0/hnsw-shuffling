#include "./hnswlib/hnswlib.h"
#include "./generate_data/generate_random_data.h"
#include <iostream>
#include <chrono>

#define SEED 30

int main() {
    using namespace std::chrono;

    int dim = 1024;             // Dimension of the elements
    int max_elements = 100000;   // Maximum number of elements, should be known beforehand
    int M = 16;                 // Tightly connected with internal dimensionality of the data
                                // strongly affects the memory consumption
    int ef_construction = 200;  // Controls index search speed/build speed tradeoff

    // Initialize index
    hnswlib::L2Space space(dim);
    hnswlib::HierarchicalNSW<float>* alg_hnsw = new hnswlib::HierarchicalNSW<float>(&space, max_elements, M, ef_construction);

    // Generate random data
    auto start = high_resolution_clock::now();
    float* data = generate_random_cluster_data(dim, max_elements, SEED);
    shuffle_data(data, dim, max_elements, SEED);
    auto end = high_resolution_clock::now();
    std::cout << "Data generation time: " << duration_cast<milliseconds>(end - start).count() << " ms" << std::endl;

    // Add data to index
    start = high_resolution_clock::now();
    for (int i = 0; i < max_elements; i++) {
        alg_hnsw->addPoint(data + i * dim, i);
    }
    end = high_resolution_clock::now();
    std::cout << "Index building time: " << duration_cast<milliseconds>(end - start).count() << " ms" << std::endl;

    // Query the elements and measure times
    long long min_query_time = LLONG_MAX;
    long long max_query_time = LLONG_MIN;

    float correct = 0;
    auto query_total_start = high_resolution_clock::now();
    for (int i = 0; i < max_elements; i++) {
        auto query_start = high_resolution_clock::now();
        std::priority_queue<std::pair<float, hnswlib::labeltype>> result = alg_hnsw->searchKnn(data + i * dim, 1);
        auto query_end = high_resolution_clock::now();

        hnswlib::labeltype label = result.top().second;
        if (label == i) correct++;

        long long query_time = duration_cast<microseconds>(query_end - query_start).count();
        min_query_time = std::min(query_time, min_query_time);
        max_query_time = std::max(query_time, max_query_time);
    }
    auto query_total_end = high_resolution_clock::now();

    long long total_query_time = duration_cast<microseconds>(query_total_end - query_total_start).count();

    std::cout << "Min query time: " << min_query_time << " µs" << std::endl;
    std::cout << "Max query time: " << max_query_time << " µs" << std::endl;
    std::cout << "Total query time: " << total_query_time << " µs" << std::endl;

    float recall = correct / max_elements;
    std::cout << "Recall: " << recall << "\n";

    delete[] data;
    delete alg_hnsw;
    return 0;
}
