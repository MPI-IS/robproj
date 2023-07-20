#include <pybind11/pybind11.h>
#include "robproj2/run_cos_signal.hpp"

using namespace robproj2;

// python wrappers for code in robproj2.

// IMPORTANT:
// in CMakeLists.txt, in the section "python wrappers"
// there is these lines:
//   set_target_properties(${PROJECT_NAME}_py
//   PROPERTIES PREFIX "" SUFFIX "${PYTHON_MODULE_EXTENSION}"
//   OUTPUT_NAME ${PROJECT_NAME}_wrp)
// i.e. the output name is ${PROJECT_NAME}_wrp aka 'robroj2_wrp'.
// This *must* match the module name below (PYBIND11_MODULE(robproj2_wrp, m))
// If not, imports will fail without explicit error messages.

// Once the workspace compiled and install/setup.bash sourced, it will be possible
// call in python `import robproj2_wrp`

PYBIND11_MODULE(robproj2_wrp, m) {

    // wrapping the RunCosSignalPeriods class
    pybind11::class_<RunCosSignalPeriods>(m, "RunCosSignalPeriods")
        .def(pybind11::init<const std::string &>())
        .def("ping", &RunCosSignalPeriods::ping);

    // wrapping the print report function
    m.def("print_report", &print_report);

    // wrapping the RunCosSignal class
    pybind11::class_<RunCosSignal>(m, "RunCosSignal")
        .def(pybind11::init<double, double, const std::string &, const std::string &, const std::string &>())
        .def("start", &RunCosSignal::start)
        .def("stop", &RunCosSignal::stop);
}

	     
