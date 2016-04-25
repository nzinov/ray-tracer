#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "primitive.hpp"

class Triangle : public Primitive {
    Point a, b, c;
public:
    Triangle(Color color, Point a, Point b, Point c) : Primitive(color), a(a), b(b), c(c) {}
    virtual Vector normal(Point) const {
        return vec(b - a, c - a).normalized();
    }

    virtual double intersect(Ray ray) const {
        Vector edge1 = b - a;
        Vector edge2 = c - a;
        //Begin calculating determinant - also used to calculate u parameter
        Vector P = vec(ray.direction, edge2);
        double det = dot(P, edge1);
        if (almost_zero(det)) {
            return INFINITY;
        }
        double inv_det = 1.0 / det;
        Vector move = ray.start - a;

        double u = dot(move, P) * inv_det;
        if (u < 0.0 || u > 1.0) {
            return INFINITY;
        }
        Vector Q = vec(move, edge1);
        double v = dot(ray.direction, Q) * inv_det;
        if(v < 0.0 || u + v  > 1.0) {
            return INFINITY;
        }
        double t = dot(edge2, Q) * inv_det;
        return t;
    }

    virtual Box bbox() const {
        return Box(Point(), 0, 0, 0);
    }
};
#endif
