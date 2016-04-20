#include "surface.hpp"
#include "scene.hpp"
#include <time.h>

int main()
{
    Scene s(Camera(Ray(Point(), Vector(1, 0, 0)), 1, 1));
    Surface sf(s);
    sleep(1000);
}
