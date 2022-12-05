#include "atf/atf.hpp"

#include <string>
#include <chrono>

using namespace std::string_literals;

#define FILENAME "./tmp.bin"

constexpr int N = 1260;

template<class T>
void print(const T &config) {
    std::cout << "best config:" << std::endl;
    for (auto &&[par, val] : config)
        std::cout << par << " = " << val << std::endl;
    
}

int main() {
    auto BLOCK_SIZE = atf::tuning_parameter("BLOCK_SIZE"s, atf::interval(1, N), atf::divides(N));
    auto OPT_LEVEL = atf::tuning_parameter("OPT_LEVEL"s, atf::interval(0, 3));
    auto COMPILER = atf::tuning_parameter("COMPILER"s, {"gcc", "clang"});

    auto mmm_kernel = atf::generic::cost_function(FILENAME)
                                   .compile_script("; $COMPILER -o " FILENAME
                                                   " -O$OPT_LEVEL -DBLOCK_SIZE=$BLOCK_SIZE kernels/c_kernel.c");

    auto tuning_result = atf::tuner().tuning_parameters(BLOCK_SIZE, OPT_LEVEL, COMPILER)
                                     .search_technique(atf::auc_bandit())
                                     .tune(mmm_kernel, atf::duration<std::chrono::seconds>(300));

    print(tuning_result.best_configuration());
}
