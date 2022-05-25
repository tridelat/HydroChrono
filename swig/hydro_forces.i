// hydro_forces.i - SWIG interface
 %module hydro_forces
 %{
 #include "hydro_forces.h"
 %}
   
 void testfunctionforswigandpython();

 // Parse the original header file
class LoadAllHydroForces {
public:
	LoadAllHydroForces(std::shared_ptr<ChBody> object, std::string file, std::string body_name, HydroInputs users_hydro_inputs);
};