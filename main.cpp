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
    Scene s(Camera(Ray(Point(-5, -5, -5), Vector(1, 1, 1).normalized()), Point(1, -1, 0), 1));
    //s.add_object(new Sphere(Color(10, 0, 1), Point(0, 0, 0), 1));
    //s.add_object(new Sphere(Color(0, 1, 1), Point(1, 1, 1), 1));
    //s.add_object(new Sphere(Color(1, 0, 0), Point(0, 2, 3), 1));
    //s.add_object(new Sphere(Color(1, 1, 0), Point(2, 1, 0), 1));
    //s.add_object(new Sphere(Color(0, 0, 1), Point(1, 3, 0), 1));
    //s.prepare();
    if (argc >= 2) {
        STLLoader::populate(s, argv[1]);
        s.find_best_view(0);
    }
    Surface sf(s);
    sf.event_loop();
}
