#ifndef RAY_H
#define RAY_H
#include "util.hpp"

struct Ray {
    Point start;
    Vector direction;
    public:
    Ray() {}
    Ray(Point start, Vector direction) : start(start), direction(direction) {}

    Point get_point(double t) const {
        return start + direction*t;
    }
};
#endif
