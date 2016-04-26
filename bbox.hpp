#ifndef BBOX_H
#define BBOX_H

#include "util.hpp"
#include "ray.hpp"

struct BBox {
    Point lower, upper;
    BBox(Point l = Point(), Point u = Point()) : lower(l), upper(u) {}

    std::pair<double, double> intersect(const Ray& ray) const {
        Point inv_dir(1/ray.direction.x, 1/ray.direction.y, 1/ray.direction.z);
        double lo = inv_dir.x*(lower.x - ray.start.x);
        double hi = inv_dir.x*(upper.x - ray.start.x);
        double tmin  = std::min(lo, hi);
        double tmax = std::max(lo, hi);

        double lo1 = inv_dir.y*(lower.y - ray.start.y);
        double hi1 = inv_dir.y*(upper.y - ray.start.y);
        tmin  = std::max(tmin, std::min(lo1, hi1));
        tmax = std::min(tmax, std::max(lo1, hi1));

        double lo2 = inv_dir.z*(lower.z - ray.start.z);
        double hi2 = inv_dir.z*(upper.z - ray.start.z);
        tmin  = std::max(tmin, std::min(lo2, hi2));
        tmax = std::min(tmax, std::max(lo2, hi2));
        if (tmax < tmin) {
            return {INFINITY, INFINITY};
        }

        return {tmin, tmax};
    }

    BBox& operator+=(const BBox& other) {
        for (int c = 0; c < 3; ++c) {
            lower.coord[c] = std::min(lower.coord[c], other.lower.coord[c]);
            upper.coord[c] = std::max(upper.coord[c], other.upper.coord[c]);
        }
        return *this;
    }

    BBox& operator+=(const Point& point) {
        for (int c = 0; c < 3; ++c) {
            lower.coord[c] = std::min(lower.coord[c], point.coord[c]);
            upper.coord[c] = std::max(upper.coord[c], point.coord[c]);
        }
        return *this;
    }

    void debug_draw(Color c) const {
        for (double x : {lower.x, upper.x}) { 
           for (double y : {lower.y, upper.y}) { 
               draw_line(c, Point(x, y, lower.z), Point(x, y, upper.z));
           }
       }
       for (double x : {lower.x, upper.x}) { 
           for (double z : {lower.z, upper.z}) { 
               draw_line(c, Point(x, lower.y, z), Point(x, upper.y, z));
           }
       }
       for (double y : {lower.y, upper.y}) { 
           for (double z : {lower.z, upper.z}) { 
               draw_line(c, Point(lower.x, y, z), Point(upper.x, y, z));
           }
       }
    }
};
#endif
