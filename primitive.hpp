#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "util.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "bbox.hpp"
#include "material.hpp"

class Primitive {
public:
    Material* material;
    Primitive() {}
    virtual Vector normal(Point point) const = 0;

    virtual const Material& texture(Point point) const {
        return *material;
    }

    virtual double intersect(Ray ray) const = 0;
    virtual BBox bbox() const = 0;
    Primitive(const Primitive&) = delete;
    Primitive& operator=(const Primitive&) = delete;
    Primitive(Primitive&&) = default;
    Primitive& operator=(Primitive&&) = default;
    virtual ~Primitive() = default;
};
#endif


