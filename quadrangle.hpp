#ifndef QUADRANGLE_H
#define QUADRANGLE_H
#include "primitive.hpp"

class Quadrangle : public Primitive {
public:
    Point vertices[4];
    Quadrangle() {}
    Quadrangle(Point a, Point b, Point c, Point d) : vertices{a, b, c, d} {}
    virtual Vector normal(Point) const {
        return vec(vertices[1] - vertices[0], vertices[2] - vertices[0]).normalized();
    }

    virtual double intersect(Ray ray) const {
        Vector edge1 = vertices[1] - vertices[0];
        Vector edge2 = vertices[3] - vertices[0];
        //Begin calculating determinant - also used to calculate u parameter
        Vector P = vec(ray.direction, edge2);
        double det = dot(P, edge1);
        double inv_det = 1.0 / det;
        Vector move = ray.start - vertices[0];

        double u = dot(move, P) * inv_det;
        Vector Q = vec(move, edge1);
        double v = dot(ray.direction, Q) * inv_det;
        if(almost_zero(det) || u < -EPS || u > 1.0 + EPS || v < -EPS || u + v  > 1.0 + EPS) {
            Vector edge2 = vertices[1] - vertices[2];
            Vector edge1 = vertices[3] - vertices[2];
            //Begin calculating determinant - also used to calculate u parameter
            Vector P = vec(ray.direction, edge2);
            double det = dot(P, edge1);
            double inv_det = 1.0 / det;
            Vector move = ray.start - vertices[2];

            double u = dot(move, P) * inv_det;
            Vector Q = vec(move, edge1);
            double v = dot(ray.direction, Q) * inv_det;
            if(almost_zero(det) || u < -EPS || u > 1.0 + EPS || v < -EPS || u + v  > 1.0 + EPS) {
                return INFINITY;
            }
            double t = dot(edge2, Q) * inv_det;
            return t;
        }
        double t = dot(edge2, Q) * inv_det;
        return t;
    }

    virtual const BBox& bbox() const {
        BBox box(vertices[0], vertices[0]);
        box += vertices[1];
        box += vertices[2];
        box += vertices[3];
        return box;
    }
};
#endif
