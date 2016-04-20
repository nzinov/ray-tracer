#ifndef SPHERE_H
#define SPHERE_H
#include "primitive.hpp"

class Sphere : public Primitive {
    Point center;
    double radius;
public:
    Sphere(Color color, Point center, double radius) : Primitive(color), center(center), radius(radius) {}
    virtual Vector normal(Point point) const {
        return (point - center).normalized();
    }

    virtual Maybe<double> intersect(Ray ray) const {
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
            double t = (min_t >= 0) ? min_t : max_t;
            if (t > 0) {
                return t;
            }
        }
        return Maybe<double>();
    }

    virtual Box bbox() const {
        return Box(center, radius, radius, radius);
    }
};
#endif
