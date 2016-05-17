#ifndef MATERIAL_H
#define MATERIAL_H
#include "color.hpp"
struct Material {
    Color color;
    double reflectance;
    double index;
};
#endif
