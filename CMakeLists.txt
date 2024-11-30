cmake_minimum_required(VERSION 3.28)
project(pure_hnswlib)

set(CMAKE_CXX_STANDARD 17)

include_directories(hnswlib)
include_directories(generate_data)

add_executable(example
        example.cpp
        generate_data/generate_random_data.cpp
)

add_executable(auto_experiment auto_experiment.cpp)