#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "util.hpp"
#include "color.hpp"
#include "ray.hpp"

class Primitive {
    Color color;
public:
    Primitive(Color color) : color(color) {}
    virtual Vector normal(Point point) const = 0;

    virtual Color texture(Point point) {
        return color;
    }

    virtual Maybe<double> intersect(Ray ray) const = 0;
    virtual Box bbox() const = 0;
    Primitive(const Primitive&) = delete;
    Primitive& operator=(const Primitive&) = delete;
    Primitive(Primitive&&) = default;
    Primitive& operator=(Primitive&&) = default;
    virtual ~Primitive() = default;
};
#endif


