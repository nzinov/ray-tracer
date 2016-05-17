#include "util.hpp"
#include "color.hpp"
#include "surface.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "light.hpp"
#include "stl_loader.hpp"
#include <time.h>

int main(int argc, char* argv[])
{
    Scene s;
    if (argc >= 2) {
        STLLoader::populate(s, argv[1]);
        //s.add_object(new Sphere(Color(1, 0, 0), Point(0, 0, 0), 1));
        //s.add_light(Light{Point(-1, -0.5, 0.5), Color()});
        //s.prepare();
        s.find_best_view(0);
    }
    Surface sf(s);
    sf.event_loop();
}
