#include "./hnswlib/hnswlib.h"
#include "./generate_data/generate_random_data.h"
#include <iostream>
#include <chrono>

int main() {
    using namespace std::chrono;

    int dim = 1024;             // Dimension of the elements
    int max_elements = 10000;   // Maximum number of elements, should be known beforehand
    int M = 16;                 // Tightly connected with internal dimensionality of the data
                                // strongly affects the memory consumption
    int ef_construction = 200;  // Controls index search speed/build speed tradeoff

    // Initialize index
    hnswlib::L2Space space(dim);
    hnswlib::HierarchicalNSW<float>* alg_hnsw = new hnswlib::HierarchicalNSW<float>(&space, max_elements, M, ef_construction);

    // Generate random data
    std::cout << "Generate random data" << std::endl;
    auto start = high_resolution_clock::now();
    float* data = generate_random_cluster_data(dim, max_elements);
    shuffle_data(data, dim, max_elements);
    auto end = high_resolution_clock::now();
    std::cout << "Data generation time: " << duration_cast<milliseconds>(end - start).count() << " ms" << std::endl;

    // Add data to index
    std::cout << "Add data to index" << std::endl;
    start = high_resolution_clock::now();
    for (int i = 0; i < max_elements; i++) {
        alg_hnsw->addPoint(data + i * dim, i);
    }
    end = high_resolution_clock::now();
    std::cout << "Index building time: " << duration_cast<milliseconds>(end - start).count() << " ms" << std::endl;

    // Query the elements for themselves and measure recall
    std::cout << "Query the elements for themselves and measure recall" << std::endl;
    start = high_resolution_clock::now();
    float correct = 0;
    for (int i = 0; i < max_elements; i++) {
        std::priority_queue<std::pair<float, hnswlib::labeltype>> result = alg_hnsw->searchKnn(data + i * dim, 1);
        hnswlib::labeltype label = result.top().second;
        if (label == i) correct++;
    }
    end = high_resolution_clock::now();
    float recall = correct / max_elements;
    std::cout << "Recall: " << recall << "\n";
    std::cout << "Query time: " << duration_cast<milliseconds>(end - start).count() << " ms" << std::endl;

    // Serialize index
    std::cout << "Serialize index" << std::endl;
    start = high_resolution_clock::now();
    std::string hnsw_path = "hnsw.bin";
    alg_hnsw->saveIndex(hnsw_path);
    end = high_resolution_clock::now();
    std::cout << "Serialization time: " << duration_cast<milliseconds>(end - start).count() << " ms" << std::endl;
    delete alg_hnsw;

    // Deserialize index and check recall
    std::cout << "Deserialize index and check recall" << std::endl;
    start = high_resolution_clock::now();
    alg_hnsw = new hnswlib::HierarchicalNSW<float>(&space, hnsw_path);
    end = high_resolution_clock::now();
    std::cout << "Deserialization time: " << duration_cast<milliseconds>(end - start).count() << " ms" << std::endl;

    correct = 0;
    start = high_resolution_clock::now();
    for (int i = 0; i < max_elements; i++) {
        std::priority_queue<std::pair<float, hnswlib::labeltype>> result = alg_hnsw->searchKnn(data + i * dim, 1);
        hnswlib::labeltype label = result.top().second;
        if (label == i) correct++;
    }
    end = high_resolution_clock::now();
    recall = (float)correct / max_elements;
    std::cout << "Recall of deserialized index: " << recall << std::endl;
    std::cout << "Query time after deserialization: " << duration_cast<milliseconds>(end - start).count() << " ms" << std::endl;

    delete[] data;
    delete alg_hnsw;
    return 0;
}
