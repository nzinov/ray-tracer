#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "primitive.hpp"

class Triangle : public Primitive {
    Point vertices[3];
public:
    Triangle(Color color, Point a, Point b, Point c) : Primitive(color), vertices{a, b, c} {}
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
        Point lower;
        Point upper;
        for (const Point& vertex : vertices) {
            for (int c = 0; c < 3; ++c) {
                lower.coord[c] = std::min(lower.coord[c], vertex.coord[c]);
                upper.coord[c] = std::max(upper.coord[c], vertex.coord[c]);
            }
        }
        return BBox(lower,upper);
    }
};
#endif
