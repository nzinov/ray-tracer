#include "surface.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "light.hpp"
#include <time.h>

int main()
{
    Scene s(Camera(Ray(Point(), Vector(1, 0, 0)), Point(0, 1, 0), 2));
    s.add_object(std::make_unique<Sphere>(Color(1, 0, 0), Point(5, 0, 0), 1));
    s.add_object(std::make_unique<Sphere>(Color(1, 0, 1), Point(10, 2, 2), 1));
    s.add_light(Light{Point(1, -3, -3), Color()});
    Surface sf(s, 768, 768);
    sf.draw();
    sleep(1000);
}
