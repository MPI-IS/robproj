#include "robproj2/run_cos_signal.hpp"

// You may note:
// - the name of the project is 'robproj2'
// - the name of the git repo is 'robproj2'
// - the header files are in a folder /include/robproj2
// - the code is in a namespace called robproj2.
// Using the same name 'robproj2' for all these items is not technically
// mandatory, but makes life clearer and easier.

namespace robproj2
{

  // THREAD_FUNCTION_RETURN_TYPE and THREAD_FUNCTION_RETURN_VALUE
  // are replaced during compilation time, depending if the
  // code is compiled on a 'regular' machine, a xenomai machine or
  // a rt-preempt/low latency machine (because the threading API
  // is not the same on all these kernels).
  // At compilation time, the type of the machine is determined
  // via a call to 'uname -a'.
  // See: mpi_cmake_module/cmake/os_detection.cmake.
  // (mpi_cmake_module is a package expected to be in the
  // same workspace as robpro2)
  THREAD_FUNCTION_RETURN_TYPE run_helper(void *arg)
  {
    ((RunCosSignal*)arg)->run();
    return THREAD_FUNCTION_RETURN_VALUE;
  }

  RunCosSignal::RunCosSignal(double cos_period_seconds,
			     double run_period_seconds,
			     std::string segment_id,
			     std::string object_id,
			     std::string segment_id_periods)
    : signal_(cos_period_seconds),
      run_period_(run_period_seconds),
      segment_id_(segment_id),
      object_id_(object_id),
      running_(false),
      run_cos_signal_periods_(segment_id_periods)
  {
    // a shared memory with the same segment_id
    // may already exist (e.g. from a previous run)
    // It is important to cleaning up first, otherwise
    // things will hang.
    // Alternatively, directly delete the corresponding
    // file in /dev/shm
    shared_memory::clear_array(segment_id);
  }

  RunCosSignal::~RunCosSignal()
  {
    if(running_)
      {
	stop();
      }
  }
  
  void RunCosSignal::start()
  {
    // this function will create a thread that matches
    // the kernel used, i.e. xenomai, rt-preemt or lowlatence.
    // (or no realtime kernel).
    // If you are using a realtime kernel and something
    // like "Warning this thread is not going to be real time"
    // is displayed in the terminal, something is not working
    // as expected.
    // Check the output of 'uname -a' (run from a terminal)
    // and check you have 'xenomai', 'preempt-rt' or 'lowlatency'
    // in it.
    thread_.create_realtime_thread(run_helper, (void*) this);
  }

  void RunCosSignal::stop()
  {
    running_ = false;
    thread_.join();
  }

  void RunCosSignal::run()
  {
    // spinner allows to enforce the desired frequency
    real_time_tools::Spinner spinner;
    spinner.set_frequency(1./(run_period_));

    double value = 0.;
    running_ = true;
    
    while (running_)
      {
	value = signal_.get();
	// writting value in the shared memory
	shared_memory::set(segment_id_, object_id_, value);
	// writting the observed period in the shared memory
	// (see run_cos_signal_periods.hpp in same directory
	// as this file)
	run_cos_signal_periods_.ping();
	// wait the right amount of time to enforce frequency
	spinner.spin();
      }
  }
      
  
}
