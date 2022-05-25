#=============================================================================
# CMake configuration file for the Chrono Python module
#
# Cannot be used stand-alone (it's loaded by CMake config. file in parent dir.)
#=============================================================================

message(STATUS "==== HydroChrono Python module ====")

# make SWIG_EXECUTABLE variable appear in cmake gui
mark_as_advanced(CLEAR SWIG_EXECUTABLE) 

find_package(PythonLibs)

message(STATUS "finding swig...")
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
message(STATUS "SWIG executable: ${SWIG_EXECUTABLE}")
message(STATUS "SWIG_USE_FILE: ${SWIG_USE_FILE}")
SET(CMAKE_SWIG_FLAGS "")


message(STATUS "finding python...")
include_directories(${PYTHON_INCLUDE_PATH})
message(STATUS "${PYTHON_INCLUDE_PATH}")


SET_SOURCE_FILES_PROPERTIES(hydro_forces.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(hydro_forces.i PROPERTIES SWIG_FLAGS "-includeall")

SWIG_ADD_LIBRARY(py_hydrochrono LANGUAGE python SOURCES hydro_forces.i)
# set_property(TARGET py_hydrochrono PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)

SWIG_LINK_LIBRARIES(py_hydrochrono HydroChrono ${PYTHON_LIBRARIES})
