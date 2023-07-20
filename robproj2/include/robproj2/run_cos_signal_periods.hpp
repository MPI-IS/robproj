#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include "shared_memory/array.hpp"

#define SHARED_PERIODS_SIZE 10000

namespace robproj2
{

  class RunCosSignalPeriods
  {
  public:
    /**
     * @brief utility function for logging periods
     * as double (in seconds) to a shared memory 
     * array of size SHARED_PERIODS_SIZE. 
     **/
    RunCosSignalPeriods(std::string segment_id);
    /**
     * Log in the shared memory the time difference
     * measured between to calls to this function. 
     */
    void ping();
  private:
    shared_memory::array<double> periods_;
    std::chrono::steady_clock::time_point previous_;
    int index_;
  };

  /**
   * @brief "plug" to the shared memory provided by the segment_id
   * and measure the average period value, as written by an instance
   * of 'RunCosSignalPeriods'. 'period' and 'tolerance' are references
   * values to count the number of times periods outside the tolerance bound
   * are observed.
   **/
  void print_report(std::string segment_id, double period, double tolerance);
  
}
