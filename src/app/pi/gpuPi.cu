#include "gpuPi.h"

#include <chrono>
#include <curand_kernel.h>
#include <iostream>

#include "cuda/mycuda.h"


static CUDA_GLOBAL void computePi(unsigned int width, unsigned int timeSeed, unsigned int* result) {
    const auto index = blockIdx.x * blockDim.x + threadIdx.x;
    if (index >= width) return;
    curandState s;
    curand_init(timeSeed, index, 0, &s);
    float x = curand_uniform(&s) * 2 - 1;
    float y = curand_uniform(&s) * 2 - 1;
    float dist = x*x + y*y;

    const bool isInCircle = (dist <= 1.0f);
    const bool resIndex = static_cast<int>(isInCircle);
    atomicAdd(result + resIndex, 1);
}

CUDA_HOST double GpuPi::compute() {
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();

    unsigned int hostResult[2] = {0};
    unsigned int* devResult;
    cudaMalloc(&devResult, sizeof(hostResult));
    cudaMemcpy(devResult, hostResult, sizeof(hostResult), cudaMemcpyHostToDevice);

    auto bloc = 1024;
    int grid = _iter / bloc + 1;
    computePi<<<grid, bloc>>>(_iter, seed, devResult);

    cudaDeviceSynchronize();

    cudaMemcpy(hostResult, devResult, sizeof(hostResult), cudaMemcpyDeviceToHost);
    cudaFree(devResult);

    unsigned int squareCount = hostResult[0];
    unsigned int circleCount = hostResult[1];
    const auto ratio  = static_cast<double>(circleCount) / (squareCount + circleCount);
    return ratio * 4;
}
