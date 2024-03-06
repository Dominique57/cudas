#pragma once

#include "cuda/mycuda.h"

class GpuPi {
  public:
    CUDA_HOSTDEV GpuPi(unsigned long long iter)
      : _iter(iter)
    {}

    CUDA_HOST double compute();

  protected:
    unsigned long long _iter;
};