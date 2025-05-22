#include "regrep.h"
#include <iostream>
#include <chrono>

int main(int argc, char* argv[]) {
    auto options = parse_arguments(argc, argv);
    if (!options.valid) {
        std::cerr << "Usage: regrep <regex|regex1&&regex2|regex1||regex2> [--invert] [--highlight] [--replace <str>] [--count] [--before N] [--after N] [--benchmark] [--showline]" << std::endl;
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    process_input(std::cin, std::cout, options);
    auto end = std::chrono::high_resolution_clock::now();

    if (options.benchmark) {
        std::chrono::duration<double> duration = end - start;
        std::cerr << "[Benchmark] Execution time: " << duration.count() << "s\n";
    }

    return 0;
}

