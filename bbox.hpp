#ifndef BBOX_H
#define BBOX_H

#include "util.hpp"
#include "ray.hpp"

struct BBox {
    Point lower, upper;
    BBox(Point l = Point(), Point u = Point()) : lower(l), upper(u) {}

    std::pair<double, double> intersect(const Ray& ray) const {
        double tmin = -INFINITY;
        double tmax = INFINITY;
        #pragma unroll
        for (int i = 0; i < 3; ++i) {
            double inv = 1 / ray.direction.coord[i];
            double lo = (lower.coord[i] - ray.start.coord[i]) * inv;
            double hi = (upper.coord[i] - ray.start.coord[i]) * inv;
            tmin  = fmax(tmin, fmin(lo, hi));
            tmax = fmin(tmax, fmax(lo, hi));
        }

        if (tmax + EPS < tmin) {
            return {INFINITY, INFINITY};
        }

        return {tmin, tmax};
    }

    BBox& operator+=(const BBox& other) {
        #pragma unroll
        for (int c = 0; c < 3; ++c) {
            lower.coord[c] = std::min(lower.coord[c], other.lower.coord[c]);
            upper.coord[c] = std::max(upper.coord[c], other.upper.coord[c]);
        }
        return *this;
    }

    BBox& operator+=(const Point& point) {
        #pragma unroll
        for (int c = 0; c < 3; ++c) {
            lower.coord[c] = std::min(lower.coord[c], point.coord[c]);
            upper.coord[c] = std::max(upper.coord[c], point.coord[c]);
        }
        return *this;
    }

    double surface() const {
        double ans = 0;
        #pragma unroll
        for (int a = 0; a < 2; ++a) {
            for (int b = a + 1; b < 3; ++b) {
                ans += 2 * (upper.coord[a] - lower.coord[a])*(upper.coord[b] - lower.coord[b]);
            }
        }
        return ans;
    }
};
#endif
