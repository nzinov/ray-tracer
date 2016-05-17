#ifndef SPHERE_H
#define SPHERE_H
#include "primitive.hpp"

class Sphere : public Primitive {
public:
    Point center;
    double radius;
    Sphere() {}
    Sphere(Point center, double radius) : center(center), radius(radius) {}
    virtual Vector normal(Point point) const {
        return (point - center).normalized();
    }

    virtual double intersect(Ray ray) const {
        Vector move = ray.start - center;
        double a = dot(ray.direction, ray.direction);
        double b = dot(move, ray.direction);
        double c = dot(move, move) - sq(radius);
        double d = sq(b) - c*a;
        if (d >= 0) {
            double sqrtd = sqrt(d);
            double t1 = (-b + sqrtd) / a;
            double t2 = (-b - sqrtd) / a;
            double min_t = fmin(t1, t2);
            double max_t = fmax(t1, t2);
            double t = (min_t >= EPS) ? min_t : max_t;
            if (t > 0) {
                return t;
            }
        }
        return INFINITY;
    }

    virtual BBox bbox() const {
        Point diag(radius, radius, radius);
        return BBox(center - diag, center + diag);
    }
};
#endif
