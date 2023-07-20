#pragma once

#include <chrono>
#include <cmath>

namespace robproj1
{

  class CosSignal
  {
  public:
    CosSignal(double period_seconds);
    double get();
  private:
    double param_;
    std::chrono::nanoseconds time_start_;
  };
  

}
