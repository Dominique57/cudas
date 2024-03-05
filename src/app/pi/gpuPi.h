#pragma once

#include "cuda/mycuda.h"

class GpuPi {
  public:
    CUDA_HOSTDEV GpuPi(int iter)
      : _iter(iter)
    {}

    CUDA_HOSTDEV double compute();

  protected:
    int _iter;
};