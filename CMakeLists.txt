cmake_minimum_required(VERSION 3.28)
project(hnsw_shuffling_test)

set(CMAKE_CXX_STANDARD 17)

include_directories(hnswlib)
include_directories(generate_data)
include_directories(shuffle_data)

add_executable(random_data
        random_data.cpp
        generate_data/generate_random_data.cpp
        shuffle_data/shuffle_data.cpp
)

add_executable(auto_experiment auto_experiment.cpp)