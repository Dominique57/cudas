#include <benchmark/benchmark.h>
#include "app/pi/cpuPi.h"
#include "app/pi/gpuPi.h"

constexpr auto DEFAULT_ITER = 1'000'000;

void BM_cpu_1M_iter(benchmark::State& st) {
    auto cpu = CpuPi(DEFAULT_ITER);
    double cpuPi;
    for (auto _ : st) {
        benchmark::DoNotOptimize(cpuPi = cpu.compute());
    }
    st.counters["frame_rate"] = benchmark::Counter((double)st.iterations(), benchmark::Counter::kIsRate);
}

void BM_gpu_1M_iter(benchmark::State& st) {
    auto gpu = GpuPi(DEFAULT_ITER);
    double cpuPi;
    for (auto _ : st) {
        benchmark::DoNotOptimize(cpuPi = gpu.compute());
    }
    st.counters["frame_rate"] = benchmark::Counter((double)st.iterations(), benchmark::Counter::kIsRate);
}

BENCHMARK(BM_cpu_1M_iter)
    ->Unit(benchmark::kMillisecond)
    ->UseRealTime();

BENCHMARK(BM_gpu_1M_iter)
    ->Unit(benchmark::kMillisecond)
    ->UseRealTime();

BENCHMARK_MAIN();