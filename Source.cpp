#include "pandaFramework.h"
#include "pandaSystem.h"

#include "GeoMipTerrain.h"

#include "perlinNoise2.h"

#include <iostream>
#include <cmath>

#include "directionalLight.h"
#include "ambientLight.h"

#include "genericAsyncTask.h"
#include "asyncTaskManager.h"

#include "collisionPlane.h"
#include "collisionSphere.h"
#include "collisionNode.h"
#include "CollisionHandlerPusher.h"
#include "CollisionTraverser.h"

#include "cubeTerrain.h"


using namespace std;

class Panda {
private:
    double v = 0;

public:
    NodePath pandaPos;

    void set_z(PN_stdfloat x) {
        pandaPos.set_z(x);
    }

    void set_v(double x) {
        v = x;
    }

    PN_stdfloat get_z() {
        return pandaPos.get_z();
    }

    double get_v() {
        return v;
    }

};

//Create a panda
Panda panda1;
// The global clock
PT(ClockObject) globalClock = ClockObject::get_global_clock();
// The global task manager
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();

// And the task, outside main:
AsyncTask::DoneStatus CalcPhys(GenericAsyncTask* task, void* data) {
    double time = globalClock->get_frame_time();
    time = time / 100;
    globalClock->tick();
    panda1.set_z(panda1.get_z() + panda1.get_v() * time + (-9.8 * pow(time, 2)/2));
    panda1.set_v(panda1.get_v() + -9.8 * time);
    return AsyncTask::DS_cont;
}

int main(int argc, char* argv[]) {

    // Open a new window framework and set the title
    PandaFramework framework;
    framework.open_framework(argc, argv);
    framework.set_window_title("My Panda3D Window");

    // Open the window
    WindowFramework* window = framework.open_window();

    // Get the camera and position it
    NodePath camera;
    camera = window->get_camera_group();
    camera.set_pos(0, 0, 10);
    camera.set_p(-45);

    //Camera control
    window->setup_trackball();

    int noiseMap[16][16];  //!!!! CURRENTLY THE ONLY SUPPORTED SIZE IS 16 !!!!
    PerlinNoise2 NoiseGen(0, 0, 16, 0);
    NoiseGen.set_scale(50);
    float noise;

    //Create and setup cubeTerrain
    for (int x1 = 0; x1 < 50; x1++) {
        for (int y1 = 0; y1 < 50; y1++) {

            for (int x = 0; x < 16; x++) {
                for (int y = 0; y < 16; y++) {
                    noise = NoiseGen.noise(x + x1 * 16, y + y1 * 16);
                    noise = noise * 50 + 50;
                    noiseMap[x][y] = noise;
                }
            }

            cubeTerrain terrain1;
            terrain1.set_noise_map(noiseMap);
            terrain1.generate();

            NodePath terrainNode = terrain1.get_node();
            terrainNode.reparent_to(window->get_render());
            terrainNode.set_pos(16 * x1, 16 * y1, 0);
        }
    }

    //Configure the panda
    panda1.pandaPos = window->load_model(framework.get_models(), "models/panda-model");
    panda1.pandaPos.set_scale(0.001);
    panda1.pandaPos.reparent_to(window->get_render());
    panda1.pandaPos.set_pos(10, 10, 10);

    //Enable lighting
    PT(DirectionalLight) d_light;
    d_light = new DirectionalLight("my d_light");
    NodePath dlnp = window->get_render().attach_new_node(d_light);
    dlnp.set_hpr(0, -45, 0);
    window->get_render().set_light(dlnp);

    PT(AmbientLight) a_light;
    a_light = new AmbientLight("my a_light");
    a_light->set_color(LColor(0.2, 0.2, 0.2, 1));
    NodePath alnp = window->get_render().attach_new_node(a_light);
    window->get_render().set_light(alnp);

    //Enable panda physics
    //taskMgr->add(new GenericAsyncTask("Calculate physics", &CalcPhys, nullptr));

    /*
    //Create floor collision plane
    PT(CollisionPlane) cs = new CollisionPlane(LPlane(0, 0, 0));
    CollisionNode* cnode = new CollisionNode("Plane");
    cnode->add_solid(cs);
    NodePath planeColNode;
    planeColNode.attach_new_node(cnode);
    planeColNode.reparent_to(window->get_render());

    //Create panda collision plane
    PT(CollisionSphere) pn = new CollisionSphere(0, 0, 0, 1);
    CollisionNode* pnnode = new CollisionNode("Sphere");
    pnnode->add_solid(pn);
    NodePath pandaColNode;
    pandaColNode.attach_new_node(pnnode);
    pandaColNode.reparent_to(panda1.pandaPos);
    
    //Add colliders
    PT(CollisionHandlerPusher) pandaPusher = new CollisionHandlerPusher;
    pandaPusher->add_collider(pandaColNode, panda1.pandaPos);

    PT(CollisionTraverser) cTrav = new CollisionTraverser;
    cTrav->add_collider(pandaColNode, pandaPusher);
    */

    // Run the engine.
    framework.main_loop();

    //Shut down
    framework.close_framework();
    return 0;

}