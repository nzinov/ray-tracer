#include "surface.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "light.hpp"
#include "stl_loader.hpp"
#include <time.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return 0;
    }
    Scene s(Camera(Ray(Point(-3, 0.5, 0.5), Vector(1, 0, 0)), Point(0, 1, 0), 0.5));
    s.add_light(Light{Point(-2, 0, 0), Color()});
    for (int i = 1; i < 15; ++i) {
        STLLoader::populate(s, argv[1], i);
        Surface sf(s);
        sf.event_loop();
    }
}
