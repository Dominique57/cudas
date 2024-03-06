#pragma once

class CpuPi {
  public:
    CpuPi(unsigned long long iter)
      : _iter(iter)
    {}

    double compute();

  protected:
    unsigned long long _iter;
};