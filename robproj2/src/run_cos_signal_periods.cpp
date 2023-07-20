#include "robproj2/run_cos_signal_periods.hpp"

namespace robproj2
{

  RunCosSignalPeriods::RunCosSignalPeriods(std::string segment_id)
    : periods_(segment_id, SHARED_PERIODS_SIZE, true, true), // create the shared memory array. ''
                                                             // true: lock is used to allow async
                                                             //        access (from other thread)
                                                             // true: delete the segment id on exit
      previous_(std::chrono::steady_clock::now()),
      index_(0)
  {
    // init the shared memory array with -1 
    for(int i=0; i<SHARED_PERIODS_SIZE; i++){
      periods_.set(i,-1.);
    }
  }

  void RunCosSignalPeriods::ping(){
    // compute the time difference since the last call to ping and
    // write it in the shared memory array
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff =
      std::chrono::duration_cast<std::chrono::duration<double>>(now - previous_);
    previous_ = now;
    periods_.set(index_, diff.count());
    index_++;
    if(index_>=SHARED_PERIODS_SIZE)
      {
	// rotating memory
	index_ = 0;
      }
  }

  void print_report(std::string segment_id, double period, double tolerance)
  {

    // read all the values from the shared memory array and compute the
    // average measured period and number of outliers
    
    double sum = 0;
    int nb_points = 0;
    int outliers = 0;

    shared_memory::array<double> periods(segment_id, SHARED_PERIODS_SIZE,
				      false, true);

    double value;
    
    for(int i=0; i<SHARED_PERIODS_SIZE; i++)
      {
	periods.get(i, value);
	if (value>0)
	  {
	    sum += value;
	    nb_points++;
	    if(value<period-tolerance || value>period+tolerance)
	      {
		outliers+=1;
	      }
	  }
      }
    
    float average = static_cast<float>(sum)/static_cast<float>(nb_points);
    std::cout << "average period: ";
    std::cout << std::fixed << std::setprecision(5) << average << std::endl; 
    std::cout << "outliers: " << outliers << " (" << nb_points << ")" <<  std::endl;
     
  }

}
