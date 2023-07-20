/**
 * @file
 * @brief Demo on how to use the SignalHandler class.
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 * @license BSD 3-clause
 */
#include <iostream>
#include <thread>
#include "signal_handler/signal_handler.hpp"
#include "robproj1/cos_signal.hpp"

int main()
{

    // cos period of 2 seconds
    float period = 2.;

    // printing every 0.5 seconds
    std::chrono::nanoseconds sleep_duration(static_cast<int>(5e8));

    // for computing the cos signal
    robproj1::CosSignal cos_signal(period);

    // to be able to exit this executable with ctrl+c
    signal_handler::SignalHandler::initialize();

    std::cout << "Press Ctrl+C to send SIGINT" << std::endl;
    while (true)
    {
      
      std::cout<< cos_signal.get() << std::endl;
      std::this_thread::sleep_for(sleep_duration);
      // has_received_sigint() returns true if a SIGINT was received since
      // initialize() was called.
      if (signal_handler::SignalHandler::has_received_sigint())
        {
	  std::cout << "Received SIGINT" << std::endl;
	  break;
        }
    }

    return 0;
}
