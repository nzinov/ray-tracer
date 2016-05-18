#include "util.hpp"
#include "color.hpp"
#include "surface.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "light.hpp"
#include "stl_loader.hpp"
#include "rt_loader.hpp"
#include <time.h>
#include <cstring>

int main(int argc, char* argv[])
{
    Scene s;
    if (argc >= 2) {
        if (argv[1][strlen(argv[1]) - 1] == 't') {
            RTLoader loader;
            loader.populate(s, argv[1]);
        } else {
            STLLoader::populate(s, argv[1]);
            s.find_best_view(0);
        }
        //s.add_object(new Sphere(Color(1, 0, 0), Point(0, 0, 0), 1));
        //s.add_light(Light{Point(-1, -0.5, 0.5), Color()});
        //s.prepare();
    }
    Surface sf(s);
    sf.event_loop();
}
