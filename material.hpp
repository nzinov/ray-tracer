#ifndef MATERIAL_H
#define MATERIAL_H
#include "color.hpp"
struct Material {
    Color color;
    double reflectance;
    double index;
    double alpha;

    Material() : color(), reflectance(0), index(0), alpha(1) {}
};
#endif
