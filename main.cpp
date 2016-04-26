#include "util.hpp"
#include "color.hpp"
void draw_line(Color c, Point a, Point b);
#include "surface.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "light.hpp"
#include "stl_loader.hpp"
#include <time.h>

Surface* surface;
void draw_line(Color c, Point a, Point b) {
    surface->draw_line(c, a, b);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        return 0;
    }
    Scene s(Camera(Ray(Point(-5, -5, -5), Vector(1, 1, 1).normalized()), Point(1, -1, 0), 1));
    s.add_light(Light{Point(-10, 0, 20), Color()});
    //s.add_object(new Sphere(Color(1, 0, 1), Point(0, 0, 0), 1));
    //s.add_object(new Sphere(Color(0, 1, 1), Point(1, 1, 1), 1));
    //s.add_object(new Sphere(Color(1, 0, 0), Point(0, 2, 3), 1));
    //s.add_object(new Sphere(Color(1, 1, 0), Point(2, 1, 0), 1));
    //s.add_object(new Sphere(Color(0, 0, 1), Point(1, 3, 0), 1));
    STLLoader::populate(s, argv[1]);
    Surface sf(s);
    surface = &sf;
    sf.event_loop();
}
