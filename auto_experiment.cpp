#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>

#define NUM_EXECUTIONS 5

#define DIM 1024
#define MAX_ELEMENTS 10000
#define M 16
#define EF_CONSTRUCTION 200
#define SEED 47

void test(bool is_shuffling) {
    std::string command = "random_data.exe";
    command += (" " + std::to_string(DIM));
    command += (" " + std::to_string(MAX_ELEMENTS));
    command += (" " + std::to_string(M));
    command += (" " + std::to_string(EF_CONSTRUCTION));
    command += (" " + std::to_string(is_shuffling));
    command += (" " + std::to_string(SEED));

    for (int i = 1; i <= NUM_EXECUTIONS; ++i) {
        std::cout << "Execution " << i << " started." << std::endl;

        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "Execution " << i << " completed successfully." << std::endl << std::endl;
        } else {
            std::cerr << "Execution " << i << " failed with exit code: " << result << std::endl << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    std::cout << "no shuffling" << std::endl << std::endl;
    test(false);

    std::cout << "yes shuffling" << std::endl << std:: endl;
    test(true);

    return 0;
}
