#pragma once

class CpuPi {
  public:
    CpuPi(int iter)
      : _iter(iter)
    {}

    double compute();

  protected:
    int _iter;
};