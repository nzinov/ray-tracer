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
    Scene s(Camera(Ray(Point(0, 10, 10), Vector(0, -1, 0)), Point(1, 0, 0), 0.5));
    s.add_light(Light{Point(-10, 5, 20), Color()});
    //s.add_object(std::make_unique<Sphere>(Color(1, 1, 1), Point(0, 0, 0), 0.1));
    STLLoader::populate(s, argv[1]);
    Surface sf(s);
    sf.event_loop();
}
