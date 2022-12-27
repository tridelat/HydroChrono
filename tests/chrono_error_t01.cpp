#include <chrono/physics/ChLoadsBody.h>
#include <chrono/physics/ChBodyEasy.h>

#include <filesystem>  // C++17
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>

using std::filesystem::path;
static path DATADIR{};

static int init_env(int argc, char* argv[]) {
    const char* env_p = std::getenv("HYDRO_CHRONO_DATA_DIR");

    if (env_p == nullptr) {
        if (argc < 2) {
            std::cerr << "Usage: .exe [<datadir>] or set HYDRO_CHRONO_DATA_DIR environement variable" << std::endl;
            return 1;
        } else {
            DATADIR = absolute(path(argv[1]));
        }
    } else {
        DATADIR = absolute(path(env_p));
    }
    return 0;
}

int main(int argc, char* argv[]) {

    if( init_env(argc, argv) > 0) {
        std::cerr << "bad usage\n";
        return 1;
    }

    auto h5fname = (DATADIR / "rm3" / "hydroData" /"rm3.h5").lexically_normal().generic_string();
    auto b1Meshfname = (DATADIR / "rm3" / "geometry" /"float_cog.obj").lexically_normal().generic_string();
    auto b2Meshfname = (DATADIR / "rm3" / "geometry" /"plate_cog.obj").lexically_normal().generic_string();

	double density = 0.0;
    bool evaluate_mass = false;
    bool create_visu_mesh = false;
    bool detect_collision = false;
    
    auto body1 = chrono_types::make_shared<chrono::ChBodyEasyMesh>(               
		b1Meshfname,
		density,                                                                                    
		evaluate_mass,                                                                               
		create_visu_mesh,                                                                           
		detect_collision                                                                                   
		);
    auto body2 = chrono_types::make_shared<chrono::ChBodyEasyMesh>(               
		b2Meshfname, 
		density,                                                                                     
		evaluate_mass,                                                                               
		create_visu_mesh,                                                                           
		detect_collision                                                                                   
		);

    // ERROR This line produce a double free or corruption at exit (destructor)
    auto b1b2 = chrono::ChLoadBodyBodyTorque(body1, body2, {0., 0., 0.}, false);


    std::cout << "End" << std::endl;
    return 0;        

}