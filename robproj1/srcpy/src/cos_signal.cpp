#include "robproj1/cos_signal.hpp"

namespace robproj1
{

  std::chrono::nanoseconds get_time_now()
  {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
  }
  
  CosSignal::CosSignal(float period)
    : param_(2*M_PI/period),
      time_start_(get_time_now())
  {}

  CosSignal::CosSignal(float period, std::chrono::nanoseconds time_now)
    : param_(2*M_PI/period)
  {
    time_start_ = time_now;
  }

  float CosSignal::get(std::chrono::nanoseconds time_now)
  {
    long int time_diff = (time_now-time_start_).count();
    return std::cos(param_*time_diff);
  }

  float CosSignal::get(){
    return this->get(get_time_now());
  }
      

      
}
