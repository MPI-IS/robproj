#include "robproj1/cos_signal.hpp"

namespace robproj1
{

  std::chrono::nanoseconds get_time_now()
  {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
  }
  
  CosSignal::CosSignal(double period)
    : param_(2*M_PI/period),
      time_start_(get_time_now())
  {}

  double CosSignal::get(){
    std::chrono::nanoseconds time_now = get_time_now();
    double time_diff_seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(time_now-time_start_).count();
    return std::cos(param_*time_diff_seconds);
  }
      

      
}
