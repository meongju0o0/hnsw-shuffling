#include "./hnswlib/hnswlib.h"
#include "./generate_data/generate_random_data.h"
#include <iostream>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <thread>

#define NUM_THREADS 8

/*
 * argv[1]: dimension
 * argv[2]: max_elements
 * argv[3]: seed
 * argv[3]: M
 * argv[4]: ef_construction
*/

void multi_thread_build(const float* data, const int d, const int max_elements, hnswlib::HierarchicalNSW<float>* alg_hnsw) {
    std::vector<std::thread> threads;
    int elements_per_thread = max_elements / NUM_THREADS;

    for (int thread_id = 0; thread_id < NUM_THREADS; thread_id++) {
        threads.push_back(std::thread([&, thread_id] {
            const int start_label = thread_id * elements_per_thread;
            const int end_label = (thread_id == NUM_THREADS - 1) ? max_elements : start_label + elements_per_thread;

            for (int i = start_label; i < end_label; i++) {
                alg_hnsw->addPoint(data + i * d, i);
            }
        }));
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

int main(int argc, char *argv[]) {
    using namespace std::chrono;

    int dim = strtol(argv[1], nullptr, 10);             // Dimension of the elements
    int max_elements = strtol(argv[2], nullptr, 10);    // Maximum number of elements, should be known beforehand
    int M = strtol(argv[3], nullptr, 10);               // Tightly connected with internal dimensionality of the data
    // strongly affects the memory consumption
    int ef_construction = strtol(argv[4], nullptr, 10); // Controls index search speed/build speed tradeoff

    int seed = strtol(argv[5], nullptr, 10);

    // Initialize index
    hnswlib::L2Space space(dim);
    hnswlib::HierarchicalNSW<float>* alg_hnsw = new hnswlib::HierarchicalNSW<float>(&space, max_elements, M, ef_construction);

    // Generate random data
    auto start = high_resolution_clock::now();
    float* data = generate_random_cluster_data(dim, max_elements, seed);
    shuffle_data(data, dim, max_elements, seed);
    auto end = high_resolution_clock::now();
    std::cout << "Data generation time: " << duration_cast<milliseconds>(end - start).count() << " ms" << std::endl;

    // Add data to index
    multi_thread_build(data, dim, max_elements, alg_hnsw);
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
