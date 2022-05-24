// hydro_forces_no_viz.i - SWIG interface
 %module hydrochrono_py
 %{
 #include "hydro_forces.h"
 %}
 
 %ignore ""; // Ignore everything

// Unignore chosen class 'HydroInputs'
%rename("%s") HydroInputs;

// As the ignore everything will include the constructor, destructor, methods etc
// in the class, these have to be explicitly unignored too:
%rename("%s") HydroInputs::HydroInputs;
// %rename("%s") HydroInputs::~HydroInputs;
%rename("%s") HydroInputs::SetRegularWaveAmplitude; // named method
%rename("%s") HydroInputs::SetRegularWaveOmega;
%rename("%s") HydroInputs::GetRegularWaveAmplitude;
%rename("%s") HydroInputs::GetRegularWaveOmega;

%include "hydro_forces.h"


// class LoadAllHydroForces {
// public:
// 	LoadAllHydroForces(std::shared_ptr<ChBody> object, std::string file);
// };
