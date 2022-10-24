#include "../../src/hydro_forces.h"
//#include "./src/hydro_forces.h"
#include "chrono_irrlicht/ChVisualSystemIrrlicht.h"
#include "chrono_irrlicht/ChIrrMeshTools.h"
#include "chrono/core/ChRealtimeStep.h"
#include <iomanip> // std::setprecision
#include <chrono> // std::chrono::high_resolution_clock::now
#include <vector> // std::vector<double>

// Use the namespaces of Chrono
using namespace chrono;
using namespace chrono::geometry;
using namespace chrono::irrlicht;

// Use the main namespaces of Irrlicht
using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::io;
using namespace irr::gui;

class MyActionReceiver : public IEventReceiver {
public:
	MyActionReceiver(ChVisualSystemIrrlicht* vsys, bool& buttonPressed)
		: pressed(buttonPressed) {
		// store pointer application
		vis = vsys;

		// ..add a GUI button to control pause/play
		pauseButton = vis->GetGUIEnvironment()->addButton(rect<s32>(510, 20, 650, 35));
		buttonText = vis->GetGUIEnvironment()->addStaticText(L"Paused", rect<s32>(560, 20, 600, 35), false);
	}

	bool OnEvent(const SEvent& event) {
		// check if user clicked button
		if (event.EventType == EET_GUI_EVENT) {
			switch (event.GUIEvent.EventType) {
			case EGET_BUTTON_CLICKED:
				pressed = !pressed;
				if (pressed) {
					buttonText->setText(L"Playing");
				}
				else {
					buttonText->setText(L"Paused");
				}
				return pressed;
				break;
			default:
				break;
			}
		}
		return false;
	}

private:
	ChVisualSystemIrrlicht* vis;
	IGUIButton* pauseButton;
	IGUIStaticText* buttonText;

	bool& pressed;
};

int main(int argc, char* argv[]) {
	//auto start = std::chrono::high_resolution_clock::now();
	GetLog() << "Copyright (c) 2017 projectchrono.org\nChrono version: " << CHRONO_VERSION << "\n\n";

	// system/solver settings
	ChSystemNSC system;
	system.Set_G_acc(ChVector<>(0.0, 0.0, 0.0));
	double timestep = 0.02;
	system.SetTimestepperType(ChTimestepper::Type::HHT);
	system.SetSolverType(ChSolver::Type::GMRES);
	system.SetSolverMaxIterations(300);  // the higher, the easier to keep the constraints satisfied.
	system.SetStep(timestep);
	ChRealtimeStepTimer realtime_timer;
	double simulationDuration = 60.0;

	// some io/viz options
	bool visualizationOn = true;
	bool profilingOn = true;
	bool saveDataOn = true;
	std::vector<double> time_vector;
	std::vector<double> float_heave_position;
	std::vector<double> plate_heave_position;

	// set up body from a mesh
	if (!std::filesystem::exists("../../HydroChrono/demos/f3of/geometry/base.obj")) {
		std::cout << "File " << std::filesystem::absolute(GetChronoDataFile("../../HydroChrono/demos/f3of/geometry/base.obj").c_str()) << " does not exist" << std::endl;
		return 0;
	}
	//std::cout << "Attempting to open mesh file: " << std::filesystem::absolute(GetChronoDataFile("../../HydroChrono/meshFiles/float.obj").c_str()) << std::endl;
	std::shared_ptr<ChBody> base = chrono_types::make_shared<ChBodyEasyMesh>(                   //
		GetChronoDataFile("../../HydroChrono/demos/f3of/geometry/base.obj").c_str(),                 // file name
		0,                                                                                        // density
		false,                                                                                    // do not evaluate mass automatically
		true,                                                                                     // create visualization asset
		false,                                                                                    // collisions
		nullptr,                                                                                  // no need for contact material
		0                                                                                         // swept sphere radius
		);

	// set up body from a mesh
	if (!std::filesystem::exists("../../HydroChrono/demos/f3of/geometry/flap.obj")) {
		std::cout << "File " << std::filesystem::absolute(GetChronoDataFile("../../HydroChrono/demos/f3of/geometry/flap.obj").c_str()) << " does not exist" << std::endl;
		return 0;
	}

	//std::cout << "Attempting to open mesh file: " << std::filesystem::absolute(GetChronoDataFile("../../HydroChrono/meshFiles/plate.obj").c_str()) << std::endl;
	std::shared_ptr<ChBody> flapFore = chrono_types::make_shared<ChBodyEasyMesh>(                   //
		GetChronoDataFile("../../HydroChrono/demos/f3of/geometry/flap.obj").c_str(),                 // file name
		0,                                                                                        // density
		false,                                                                                    // do not evaluate mass automatically
		true,                                                                                     // create visualization asset
		false,                                                                                    // collisions
		nullptr,                                                                                  // no need for contact material
		0                                                                                         // swept sphere radius
		);

	// set up body from a mesh
	if (!std::filesystem::exists("../../HydroChrono/demos/f3of/geometry/flap.obj")) {
		std::cout << "File " << std::filesystem::absolute(GetChronoDataFile("../../HydroChrono/demos/F3OF/geometry/flap.obj").c_str()) << " does not exist" << std::endl;
		return 0;
	}

	//std::cout << "Attempting to open mesh file: " << std::filesystem::absolute(GetChronoDataFile("../../HydroChrono/meshFiles/plate.obj").c_str()) << std::endl;
	std::shared_ptr<ChBody> flapAft = chrono_types::make_shared<ChBodyEasyMesh>(                   //
		GetChronoDataFile("../../HydroChrono/demos/f3of/geometry/flap.obj").c_str(),                 // file name
		0,                                                                                        // density
		false,                                                                                    // do not evaluate mass automatically
		true,                                                                                     // create visualization asset
		false,                                                                                    // collisions
		nullptr,                                                                                  // no need for contact material
		0                                                                                         // swept sphere radius
		);


	// define the float's initial conditions
	system.Add(base);
	base->SetNameString("body1");
	base->SetPos(ChVector<>(0,0,-9));
	base->SetMass(1089825.0);
	//base->SetInertiaXX(ChVector<>(20907301.0, 21306090.66, 37085481.11));
	//base->SetCollide(false);

	// define the plate's initial conditions
	system.Add(flapFore);
	flapFore->SetNameString("body2");
	flapFore->SetPos(ChVector<>(-12.5,0,-5.5));
	flapFore->SetMass(179250.0);
	// flapFore->SetInertiaXX(ChVector<>(94419614.57, 94407091.24, 28542224.82));
	//flapFore->SetCollide(false);

	// define the float's initial conditions
	system.Add(flapAft);
	flapAft->SetNameString("body3");
	flapAft->SetPos(ChVector<>(12.5,0,-5.5));
	flapAft->SetMass(179250.0);
	// flapAft->SetInertiaXX(ChVector<>(20907301.0, 21306090.66, 37085481.11));
	//base->SetCollide(false);


	// add revolute joint between the two bodies
	//auto prismatic = chrono_types::make_shared<ChLinkLockPrismatic>();
	//auto Flap_Right_Joint = chrono_types::make_shared<ChLinkLockRevolute>();
	//ChQuaternion<> rev_rot = Q_from_AngX(CH_C_PI / 2.0); // set as 0 initially, use to displace later
	//ChVector<> rev_pos(12.5, 0, -9.0); // location of right revolute joint
	//// Create revolute joint between body and ground
	//auto rev_right = chrono_types::make_shared<ChLinkLockRevolute>();
	//rev_right->Initialize(base, flapAft, ChCoordsys<>(rev_pos, rev_rot));
	//system.AddLink(rev_right);
	//prismatic->Initialize(base, flapFore, false, ChCoordsys<>(ChVector<>(0, 0, -0.72)), ChCoordsys<>(ChVector<>(0, 0, -21.29)));
	//Flap_Right_Joint->Initialize(base, flapAft, ChCoordsys<>(ChVector<>(-12.5, 0, 0)));
	//system.AddLink(prismatic);
	//system.AddLink(Flap_Right_Joint);

	//auto prismatic_pto = chrono_types::make_shared<ChLinkTSDA>();
	//auto Flap_Left_Joint = chrono_types::make_shared<ChLinkLockRevolute>();
	//prismatic_pto->Initialize(base, flapFore, false, ChVector<>(0, 0, -0.72), ChVector<>(0, 0, -21.29));
	//Flap_Left_Joint->Initialize(base, flapFore, ChCoordsys<>(ChVector<>(12.5, 0, 0)));
	//system.AddLink(prismatic_pto);
	//system.AddLink(Flap_Left_Joint);

	// define wave parameters (not used in this demo)
	HydroInputs my_hydro_inputs;
	my_hydro_inputs.regular_wave_amplitude = 0.022;
	my_hydro_inputs.regular_wave_omega = 2.10;
	my_hydro_inputs.mode = noWaveCIC;  // switch to NONE to turn waves off
	// attach hydrodynamic forces to body,switch to REGULAR with REGULAR
	std::vector<std::shared_ptr<ChBody>> bodies;
	bodies.push_back(base);
	bodies.push_back(flapFore);
	bodies.push_back(flapAft);
	TestHydro blah(bodies, "../../HydroChrono/demos/f3of/hydroData/f3of.h5", my_hydro_inputs);

	//// Debug printing added mass matrix and system mass matrix
	//ChSparseMatrix M;
	//system.GetMassMatrix(&M);
	//std::cout << M << std::endl;

	// for profiling
	auto start = std::chrono::high_resolution_clock::now();

	if (visualizationOn) {
		// create the irrlicht application for visualizing
		auto irrlichtVis = chrono_types::make_shared<ChVisualSystemIrrlicht>();
		irrlichtVis->AttachSystem(&system);
		irrlichtVis->SetWindowSize(1280, 720);
		irrlichtVis->SetWindowTitle("F3OF - Decay Test");
		irrlichtVis->SetCameraVertical(CameraVerticalDir::Z);
		irrlichtVis->Initialize();
		irrlichtVis->AddLogo();
		irrlichtVis->AddSkyBox();
		irrlichtVis->AddCamera(ChVector<>(0, -50, -10), ChVector<>(0, 0, -10)); // camera position and where it points
		irrlichtVis->AddTypicalLights();
		irrlichtVis->EnableBodyFrameDrawing(true);
		irrlichtVis->EnableLinkFrameDrawing(true);

		// add play/pause button
		bool buttonPressed = false;
		MyActionReceiver receiver(irrlichtVis.get(), buttonPressed);
		irrlichtVis->AddUserEventReceiver(&receiver);

		// main simulation loop
		while (irrlichtVis->Run() && system.GetChTime() <= simulationDuration) {
			irrlichtVis->BeginScene();
			irrlichtVis->Render();
			irrlichtVis->EndScene();
			if (buttonPressed) {
				//system.GetMassMatrix(&M);
				//std::cout << M << std::endl;
				// step the simulation forwards
				system.DoStepDynamics(timestep);
				// append data to std vector
				time_vector.push_back(system.GetChTime());
				float_heave_position.push_back(base->GetPos().z());
				plate_heave_position.push_back(flapFore->GetPos().z());
				// force playback to be real-time
				realtime_timer.Spin(timestep);
			}
		}
	}
	else {
		int frame = 0;
		while (system.GetChTime() <= simulationDuration) {
			// append data to std vector
			time_vector.push_back(system.GetChTime());
			float_heave_position.push_back(base->GetPos().z());
			plate_heave_position.push_back(flapFore->GetPos().z());

			// step the simulation forwards
			system.DoStepDynamics(timestep);

			frame++;
		}
	}

	// for profiling
	auto end = std::chrono::high_resolution_clock::now();
	unsigned duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	if (profilingOn) {
		std::ofstream profilingFile;
		profilingFile.open("./results/f3of/decay/duration_ms.txt");
		if (!profilingFile.is_open()) {
			if (!std::filesystem::exists("./results/f3of/decay")) {
				std::cout << "Path " << std::filesystem::absolute("./results/f3of/decay") << " does not exist, creating it now..." << std::endl;
				std::filesystem::create_directory("./results");
				std::filesystem::create_directory("./results/f3of");
				std::filesystem::create_directory("./results/f3of/decay");
				profilingFile.open("./results/f3of/decay/duration_ms.txt");
				if (!profilingFile.is_open()) {
					std::cout << "Still cannot open file, ending program" << std::endl;
					return 0;
				}
			}
		}
		profilingFile << duration << "\n";
		profilingFile.close();
	}

	if (saveDataOn) {
		std::ofstream outputFile;
		outputFile.open("./results/f3of/decay/f3of_decay.txt");
		if (!outputFile.is_open()) {
			if (!std::filesystem::exists("./results/f3of/decay")) {
				std::cout << "Path " << std::filesystem::absolute("./results/f3of/decay") << " does not exist, creating it now..." << std::endl;
				std::filesystem::create_directory("./results");
				std::filesystem::create_directory("./results/f3of");
				std::filesystem::create_directory("./results/f3of/decay");
				outputFile.open("./results/f3of/decay/f3of_decay.txt");
				if (!outputFile.is_open()) {
					std::cout << "Still cannot open file, ending program" << std::endl;
					return 0;
				}
			}
		}
		outputFile << std::left << std::setw(10) << "Time (s)"
			<< std::right << std::setw(16) << "Float Heave (m)"
			<< std::right << std::setw(16) << "Plate Heave (m)"
			<< std::endl;
		for (int i = 0; i < time_vector.size(); ++i)
			outputFile << std::left << std::setw(10) << std::setprecision(2) << std::fixed << time_vector[i]
			<< std::right << std::setw(16) << std::setprecision(4) << std::fixed << float_heave_position[i]
			<< std::right << std::setw(16) << std::setprecision(4) << std::fixed << plate_heave_position[i]
			<< std::endl;
		outputFile.close();
	}
	return 0;
}