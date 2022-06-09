#=============================================================================
# CMake configuration file for the Chrono Python module
#
# Cannot be used stand-alone (it's loaded by CMake config. file in parent dir.)
#=============================================================================

message(STATUS "==== HydroChrono Python module ====")

# make SWIG_EXECUTABLE variable appear in cmake gui
mark_as_advanced(CLEAR SWIG_EXECUTABLE) 

# set package name
set(HYDROPY_PACKAGENAME py_hydrochrono)

# find python
message(STATUS "finding python...")
find_package(PythonLibs)

set(Python_ADDITIONAL_VERSIONS 3.4)
find_package(Python3 REQUIRED)
find_package(PythonLibs)

get_filename_component(HYDRO_PYTHONDIR "${PYTHON_EXECUTABLE}" PATH)
set(HYDRO_PYTHONINC "${PYTHON_INCLUDE_DIR}")
set(HYDRO_PYTHONLIB "${PYTHON_LIBRARIES}")

set(HYDRO_PYTHONDIR  "${HYDRO_PYTHONDIR}"  PARENT_SCOPE)
set(HYDRO_PYTHONINC  "${HYDRO_PYTHONINC}"  PARENT_SCOPE)
set(HYDRO_PYTHONLIB  "${HYDRO_PYTHONLIB}"  PARENT_SCOPE)

include_directories(${HYDRO_PYTHONINC})

# find swig
message(STATUS "finding swig...")

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

SET(CMAKE_SWIG_FLAGS "") # chrono sets -c++ here and disables a few swig warnings as well....
# I'll set these this way for now...
SET_SOURCE_FILES_PROPERTIES(hydro_forces.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(hydro_forces.i PROPERTIES SWIG_FLAGS "-includeall")

# optionally set swig locations here 
set(CMAKE_SWIG_OUTDIR "${PROJECT_BINARY_DIR}/hydrochrono_python")

# optionally diable more c++ warnings here from swig stuff

# Module for hydro chrono hydro_forces:
# Python module name:
set(HYDROPY hydro_forces)

# interface files 
set(HYDROPY_MODULE_FILE
    hydro_forces.i
    )
# set some header file only stuff here? not sure if needed...

# Create the SWIG module
SWIG_ADD_LIBRARY(${HYDROPY} TYPE SHARED LANGUAGE python SOURCES ${HYDROPY_MODULE_FILE})
target_include_directories(${HYDROPY} PRIVATE ${HYDRO_PYTHONINC})
SWIG_LINK_LIBRARIES(${HYDROPY} ${PYTHON_LIBRARIES} ${LINK_LIBS} HydroChrono)

# set_target_properties?
set_target_properties(${SWIG_MODULE_${HYDROPY}_REAL_NAME} PROPERTIES
                      PROJECT_LABEL "HydroChrono_python_${HYDROPY}"
                      OUTPUT_NAME   "${SWIG_MODULE_${HYDROPY}_REAL_NAME}"
                      LIBRARY_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}"
                      )

set_property(TARGET ${HYDROPY} PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE)

# target_compile_definitions?

# add_dependencies now (seems important for finding libs)
# just makes sure the add_library(HydroChrono) gets called before doing the swig stuff
ADD_DEPENDENCIES(${SWIG_MODULE_${HYDROPY}_REAL_NAME} HydroChrono)

# # copy py file in a package dir
# set(HYDROPY_PY_FILE "${CMAKE_SWIG_OUTDIR}/${HYDROPY}.py")
# get_target_property(HYDROPY_PYD_FILE ${SWIG_MODULE_${HYDROPY}_REAL_NAME} LOCATION)
# get_filename_component(HYDROPY_PYD_PATH "${HYDROPY_PYD_FILE}" PATH)
# add_custom_command(TARGET ${SWIG_MODULE_${HYDROPY}_REAL_NAME}
#                    POST_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E make_directory ${HYDROPY_PYD_PATH}/${HYDROPY_PACKAGENAME})
# add_custom_command(TARGET ${SWIG_MODULE_${HYDROPY}_REAL_NAME}
#                    POST_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E copy ${HYDROPY_PY_FILE} ${HYDROPY_PYD_PATH}/${HYDROPY_PACKAGENAME})

# # Generate the __init__.py file using substitution variables and place it in a temporary place
# configure_file(${CMAKE_SOURCE_DIR}/contrib/packaging-python/__init__.py.in
#                ${PROJECT_BINARY_DIR}/hydrochrono_python/__init__.py)

# # Copy the __init__.py file in the package
# add_custom_command(TARGET ${SWIG_MODULE_${HYDROPY}_REAL_NAME}
#                    POST_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_BINARY_DIR}/chrono_python/__init__.py" ${CHPY_CORE_PYD_PATH}/${CHPY_PACKAGENAME})



# use SWIG_MODULE_<name>_EXTRA_DEPS,,,, i think project chrono does this?
#
 # ADD_DEPENDENCIES(${SWIG_MODULE_${CHPY_CASCADE}_REAL_NAME} ChronoEngine)
  #ADD_DEPENDENCIES(${SWIG_MODULE_${CHPY_CASCADE}_REAL_NAME} ChronoEngine_cascade)
  #ADD_DEPENDENCIES(${SWIG_MODULE_${CHPY_CASCADE}_REAL_NAME} ${SWIG_MODULE_${HYDROPY}_REAL_NAME})

#back to required things

