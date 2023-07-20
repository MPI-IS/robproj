#pragma once

#include <chrono>
#include <cmath>

namespace robproj1
{

  class CosSignal
  {
  public:
    CosSignal(float period_ns);
    CosSignal(float period_ns, std::chrono::nanoseconds time_start);
    float get();
    float get(std::chrono::nanoseconds time_now);
  private:
    float param_;
    std::chrono::nanoseconds time_start_;
  };
  

}
