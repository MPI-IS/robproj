cmake_minimum_required(VERSION 3.10.2)
project(robproj2)

# name of the project: robproj2.
# should be consistent with the name of the project
# in package.xml (in same folder)

# in the package.xml, some dependencies are declared
# (e.g. robproj1). This ensures 'colcon build' compiles
# the packages in the right order, i.e. robproj1 will
# be compiled before robproj2.

# Using C++17
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()


################
# Dependencies #
################

# This find the libraries code
# in this package depends on.
# "find" means:
# - get the path to the include folder
# - get the path to the compiled libraries
# "get" here means that some cmake variable
# are created with the corresponding path as value.
# All these packages should be in the workspace.
# E.g. robproj1 is found because
# robproj1/CMakeLists.txt 


# if you would like to link against libraries not
# in the workspace, but globally installed (e.g. via 'apt-get')
# you need some cmake macro that will find the corresponding
# includes and libraries. For example, the package shared_memory
# needs 'cereal' (see shared_memory/CMakeLists.txt). This is solved
# as follow:
# - there is a file Findcereal.cmake in the package mpi_cmake_modules
#   (which should also be in the workspace), under the path:
#   mpi_cmake_modules/cmake/find_modules
# - shared_memory find the package "mpi_cmake_modules" before
#   finding the package cereal.

find_package(pybind11 REQUIRED)
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(mpi_cmake_modules REQUIRED)
find_package(shared_memory REQUIRED)
find_package(real_time_tools REQUIRED)
find_package(robproj1 REQUIRED)

# If a package depends on robproj2, this
# will make these dependencies also available
# to this other package
ament_export_dependencies(
  mpi_cmake_modules
  pybind11
  shared_memory
  real_time_tools
  robproj1
)

# prepare to export all needed targets
set(all_targets)
set(all_target_exports)


###########
# Library #
###########

# we create a new library called robproj2

# adding the source files
add_library( ${PROJECT_NAME} SHARED
  src/run_cos_signal.cpp
  src/run_cos_signal_periods.cpp
  )

# to find the headers in the /include/robproj2 folder 
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

# find the headers and the libraries path of these packages.
# It works because we called 'find_package' above
target_link_libraries(${PROJECT_NAME} shared_memory::shared_memory)
target_link_libraries(${PROJECT_NAME} real_time_tools::real_time_tools)
target_link_libraries(${PROJECT_NAME} robproj1::robproj1)

# this will make the libraries and the include folders
# of the robproj2 package available to the packages that
# depends on it
ament_export_interfaces(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)

list(APPEND all_targets ${PROJECT_NAME})
list(APPEND all_target_exports export_${PROJECT_NAME})


###################
# Python wrappers #
###################

# create python packages wrapping the c++ code.
# See srcpy/wrappers.cpp for more comments.
# Lines below compile the c++ code doing the wrapping,
# bind it its dependencies, create the python package
# and install it.

add_library(${PROJECT_NAME}_py MODULE srcpy/wrappers.cpp)
target_link_libraries(${PROJECT_NAME}_py PRIVATE pybind11::module)
target_link_libraries(${PROJECT_NAME}_py PRIVATE ${PYTHON_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_py PRIVATE ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}_py
  PROPERTIES PREFIX "" SUFFIX "${PYTHON_MODULE_EXTENSION}"
  OUTPUT_NAME ${PROJECT_NAME}_wrp)
target_include_directories(
  ${PROJECT_NAME}_py
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include> SYSTEM
  PUBLIC ${PYTHON_INCLUDE_DIRS})
_ament_cmake_python_get_python_install_dir()
install(TARGETS ${PROJECT_NAME}_py DESTINATION ${PYTHON_INSTALL_DIR})


######################
# Python Native code #
######################

# see python/robproj2/__init__.py for more explanation

ament_python_install_package(${PROJECT_NAME} PACKAGE_DIR python/${PROJECT_NAME})


###############
# Executables #
###############

# the files in the demos folder can directly be executed.
# The cmake code below also copy the files to the install/robproj2
# folder, so that they find themselves in the $PATH once
# install/setup.bash is sourced.

configure_file(${CMAKE_CURRENT_LIST_DIR}/demos/run_cos
  ${CMAKE_INSTALL_PREFIX}/bin/robproj2_run_cos COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/demos/plot_cos_values
  ${CMAKE_INSTALL_PREFIX}/bin/robproj2_plot_cos_values COPYONLY)
configure_file(${CMAKE_CURRENT_LIST_DIR}/demos/period_report
  ${CMAKE_INSTALL_PREFIX}/bin/robproj2_period_report COPYONLY)


######################
# Install and export #
######################

# This makes sure files are copied to the correct location
# when being installed

install(DIRECTORY include/ DESTINATION include)
install(
  TARGETS ${all_targets}
  EXPORT ${all_target_exports}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES
  DESTINATION include)

# does various magic based on everything above
ament_package()
