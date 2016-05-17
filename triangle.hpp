#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "primitive.hpp"

class Triangle : public Primitive {
public:
    Point vertices[3];
    Triangle() {}
    Triangle(Point a, Point b, Point c) : vertices{a, b, c} {}
    virtual Vector normal(Point) const {
        return vec(vertices[1] - vertices[0], vertices[2] - vertices[0]).normalized();
    }

    virtual double intersect(Ray ray) const {
        Vector edge1 = vertices[1] - vertices[0];
        Vector edge2 = vertices[2] - vertices[0];
        //Begin calculating determinant - also used to calculate u parameter
        Vector P = vec(ray.direction, edge2);
        double det = dot(P, edge1);
        if (almost_zero(det)) {
            return INFINITY;
        }
        double inv_det = 1.0 / det;
        Vector move = ray.start - vertices[0];

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

    virtual BBox bbox() const {
        BBox box(vertices[0], vertices[1]);
        box += vertices[1];
        box += vertices[2];
        return box;
    }
};
#endif
