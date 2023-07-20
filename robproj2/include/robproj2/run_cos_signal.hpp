#pragma once
#include "real_time_tools/thread.hpp"
#include "real_time_tools/spinner.hpp"
#include "shared_memory/shared_memory.hpp"
#include "robproj1/cos_signal.hpp"
#include "robproj2/run_cos_signal_periods.hpp"

namespace robproj2
{

  class RunCosSignal
  {
  public:
    /**
     * @brief This will spawn a process which will write at
     * a period of 'run_period_seconds' double values to the 
     * 'segment_id' / 'object_id' shared memory segment.
     * The values are the output of a cosinus function with a period
     * of 'cos_period_seconds'.
     * 'segment_id_periods' is the segment id of a shared memory
     * array into which at each iteration the observed period will be written.
     * (useful to have another process checking to what extend 'run_period_seconds'
     * is respected)
     */
    RunCosSignal(double cos_period_seconds, double run_period_seconds,
		 std::string segment_id, std::string object_id,
		 std::string segment_id_periods);
    ~RunCosSignal();
    void start();
    void stop();
    void run();
  private:
    robproj1::CosSignal signal_;
    double run_period_;
    std::string segment_id_;
    std::string object_id_;
    real_time_tools::RealTimeThread thread_;
    std::atomic<bool> running_;
    RunCosSignalPeriods run_cos_signal_periods_;
  };
  
}
