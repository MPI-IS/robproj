#include <pybind11/pybind11.h>
#include <pybind11/chrono.h>
#include "robproj1/cos_signal.hpp"

using namespace robproj1;

PYBIND11_MODULE(robproj1, m) {
    pybind11::class_<robproj1::CosSignal>(m, "CosSignal")
        .def(pybind11::init<double>(), pybind11::arg("period_us"))
        .def("get", &robproj1::CosSignal::get);
}


	     
