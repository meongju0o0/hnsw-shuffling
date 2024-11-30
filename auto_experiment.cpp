#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

#define NUM_EXECUTIONS 5

int main() {
    const std::string command = "single_thread_example.exe";

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

    std::cout << "All executions completed." << std::endl;
    return 0;
}
