cmake_minimum_required(VERSION 3.28)
project(pure_hnswlib)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include directories for header files
include_directories(hnswlib)
include_directories(generate_data)

# Add the executable with multiple source files
add_executable(test
        example_search.cpp
        generate_data/generate_random_data.cpp
)
