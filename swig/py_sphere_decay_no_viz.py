import sys
import math
import pychrono as chrono
#import pychrono.irrlicht as chronoirr
import hydrochrono_py

# create the system
system = chrono.ChSystemNSC()
system.Set_G_acc(chrono.ChVectorD(0, 0, -9.81))

# define the sphere rigid body
body = chrono.ChBodyEasySphere(5,1)
sph = chrono.ChSphereShape()
sph.GetSphereGeometry().rad = 5
body.AddAsset(sph)

body.SetMass(261.8e3)
body.SetPos(chrono.ChVectorD(0, 0, -1))
system.AddBody(body)


# visualization
# application = chronoirr.ChIrrApp(system, "sphere-testing", chronoirr.dimension2du(400, 300))
# application.AddTypicalLights()
# application.AddCamera(chronoirr.vector3df(0, 10, -1),   # camera location
# 							 chronoirr.vector3df(0., 0., 0.))   # "look at" location
# application.AssetBindAll()
# application.AssetUpdateAll()

test = HydroInputs()
test.SetRegularWaveAmplitude(5)
# x = _py_wec_chrono.new_LoadAllHydroForces(body, str("sphere.h5"))
# blah = pyexample.TestClass()

with open("../test_for_chrono_build/Release/outfile/py_out.txt", mode='w', encoding='utf-8') as f:
    # f.write(blah.getx())
    # simulation loop
    f.write("#Time	Body Pos	Body vel (heave)	force (heave)")
    # application.SetTimestep(0.001)
    while (system.GetChTime() <= 40) :
        f.write("\n")
        f.write(str(system.GetChTime()))
        f.write("\t")
        f.write(str(body.GetPos().z))
        system.DoStepDynamics(0.015)

