#=============================================================================
# CMake configuration file for the Chrono Python module
#
# Cannot be used stand-alone (it's loaded by CMake config. file in parent dir.)
#=============================================================================

# if(NOT ENABLE_MODULE_PY)
#   mark_as_advanced(FORCE SWIG_EXECUTABLE)
#   return()
# endif()

message(STATUS "==== HydroChrono Python module ====")
mark_as_advanced(CLEAR SWIG_EXECUTABLE)

message(STATUS "finding python...")
find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})

message(STATUS "finding swig...")
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
SET(CMAKE_SWIG_FLAGS "")
SET_SOURCE_FILES_PROPERTIES(hydro_forces.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(hydro_forces.i PROPERTIES SWIG_FLAGS "-includeall")
SWIG_ADD_LIBRARY(hydrochrono_py LANGUAGE python SOURCES hydro_forces.i)
set_property(TARGET hydrochrono_py PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)

# target_link_libraries(HydroChrono ${LINK_LIBS})
# target_link_libraries(sphere_decay_no_viz HydroChrono)
# target_link_libraries(sphere_decay_demo HydroChrono)
SWIG_LINK_LIBRARIES(hydrochrono_py HydroChrono ${PYTHON_LIBRARIES})
