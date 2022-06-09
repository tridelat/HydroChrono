// hydro_forces.i - SWIG interface
 
%module hydro_forces

// Include other .i configuration files for SWIG. 
// These are divided in many .i files, each per a
// different c++ class, when possible.

%include "std_string.i"
%include "std_vector.i"
%include "typemaps.i"

// Turn on the exception handling to intercept C++ exceptions
%include "exception.i"

%exception {
  try {
    $action
  } catch (const std::exception& e) {
    SWIG_exception(SWIG_RuntimeError, e.what());
  }
}

// For optional downcasting of polimorphic objects:
%include "chrono_downcast.i" 

// For supporting shared pointers:
%include <std_shared_ptr.i>



// Include C++ headers...
%{

#include <vector>


#include "chrono/solver/ChSolverPMINRES.h"
#include "chrono/solver/ChIterativeSolverLS.h"
#include "chrono/timestepper/ChTimestepper.h"

#include "chrono/physics/ChForce.h"
#include "chrono/physics/ChLoadContainer.h"
#include "chrono/physics/ChLoadsBody.h"
#include "chrono/physics/ChSystemNSC.h"
#include "chrono/physics/ChBody.h"
#include "chrono/physics/ChBodyEasy.h"

#include "chrono/fea/ChMeshFileLoader.h"

#include "chrono/assets/ChPointPointDrawing.h"
#include "chrono_irrlicht/ChIrrApp.h"
#include "chrono_irrlicht/ChIrrMeshTools.h"

#include "H5Cpp.h"

using namespace chrono;
using namespace chrono::irrlicht;
using namespace chrono::fea;

%}


// This is to enable references to double,int,etc. types in function parameters
%pointer_class(int,int_ptr);
%pointer_class(double,double_ptr);
%pointer_class(float,float_ptr);

//For Each Class update A,B,C below
/////////////////////////////////////////////////////////////////////////////

// A - Enable shared pointers for each class...like this?
shared_ptr(H5FileInfo)
shared_ptr(HydroInputs)
shared_ptr(ForceTorqueFunc)
shared_ptr(HydroForces)
shared_ptr(ChLoadAddedMass)
shared_ptr(LoadAllHydroForces)

//deal with templates here...not needed for this proj

// B - Include headers
%include "../hydro_forces.h"
%include <vector>
%include "chrono/solver/ChSolverPMINRES.h"
%include "chrono/solver/ChIterativeSolverLS.h"
%include "chrono/timestepper/ChTimestepper.h"
%include "chrono/physics/ChForce.h"
%include "chrono/physics/ChLoadContainer.h"
%include "chrono/physics/ChLoadsBody.h"
%include "chrono/physics/ChSystemNSC.h"
%include "chrono/physics/ChBody.h"
%include "chrono/physics/ChBodyEasy.h"
%include "chrono/fea/ChMeshFileLoader.h"
%include "chrono/assets/ChPointPointDrawing.h"
%include "chrono_irrlicht/ChIrrApp.h"
%include "chrono_irrlicht/ChIrrMeshTools.h"
%include "H5Cpp.h"


//will need this for inherited classes to down/upcast. to upcast just swap base and derived
//%DefSharedPtrDynamicDowncast(base, derived)

// not sure why irrlicht module uses this syntax for downcasting these
//%DefSharedPtrDynamicDowncast2NS(chrono, chrono::irrlicht, ChAsset,ChIrrNodeAsset)
