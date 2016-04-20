#include "surface.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include <time.h>

int main()
{
    Scene s(Camera(Ray(Point(), Vector(1, 0, 0)), Point(0, 1, 0), 1, 1));
    s.add_object(std::make_unique<Sphere>(Color(1, 0, 0), Point(5, 0, 0), 1));
    Surface sf(s, 1024, 768);
    sf.draw();
    sleep(1000);
}
