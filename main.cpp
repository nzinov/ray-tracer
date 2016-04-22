#include "surface.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "light.hpp"
#include <time.h>

int main()
{
    Scene s(Camera(Ray(Point(), Vector(1, 0, 0)), Point(0, 1, 0), 1));
    s.add_object(std::make_unique<Sphere>(Color(1, 0, 0), Point(5, 2, 2), 1));
    s.add_object(std::make_unique<Triangle>(Color(1, 0, 0), Point(2, 0, 0), Point(3, 0.5, 0), Point(2, 0, 0.5)));
    s.add_light(Light{Point(0, 0, 0), Color()});
    Surface sf(s);
    sf.event_loop();
}
