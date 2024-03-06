#include "cpuPi.h"

#include <chrono>
#include <iostream>
#include <random>

double CpuPi::compute() {
    auto inSquare = 0llu;
    auto inCircle = 0llu;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    auto unif = std::uniform_real_distribution<double>(-1,1);
    for (auto i = 0llu; i < _iter; ++i) {
        double x = unif(gen);
        double y = unif(gen);
        const bool isCircle = (x * x) + (y * y) <= 1;
        if (isCircle) {
            inCircle += 1;
        } else {
            inSquare += 1;
        }
    }
    const auto ratio  = static_cast<double>(inCircle) / (inSquare + inCircle);
    return ratio * 4;
    // return 3.14159265358979323846;
}
