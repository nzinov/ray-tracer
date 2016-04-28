#ifndef KDTREE_H
#define KDTREE_H

#include "primitive.hpp"
#include "util.hpp"
#include "ray.hpp"
#include "bbox.hpp"
#include <vector>
#include <algorithm>

struct Intersection {
    double t;
    const Primitive* object;

    Intersection() : t(INFINITY), object(nullptr) {}
    Intersection(double t, const Primitive* p) : t(t), object(p) {}
};

struct Node {
    double coord;
    Node* left;
    Node* right;
    std::vector<Primitive*> objects;

    inline bool is_leaf() const {
        return !left;
    }

    Intersection intersect_contents(const Ray& ray) const {
        const Primitive* closest;
        double min_dist = INFINITY;
        for (auto obj = objects.begin(); obj != objects.end(); ++obj) {
            double dist = (*obj)->intersect(ray);
            if (!std::isinf(dist) && dist > EPS && dist < min_dist) {
                min_dist = dist;
                closest = *obj;
            }
        }
        return {min_dist, closest};
    }

    inline short next(short dim) const {
        return (dim + 1) % 3;
    }

    Intersection intersect(const Ray& ray, double t_enter, double t_leave, short dim) const {
        if (is_leaf()) {
            return intersect_contents(ray);
        }
        double t_split = (coord - ray.start.coord[dim]) / ray.direction.coord[dim];
        Node* lower = left;
        Node* upper = right;
        Intersection nearest;
        if (ray.direction.coord[dim] < 0) {
            std::swap(lower, upper);
        }
        if (t_split < t_enter) {
            nearest = upper->intersect(ray, t_enter, t_leave, next(dim));
        } else if (t_split > t_leave) {
            nearest = lower->intersect(ray, t_enter, t_leave, next(dim));
        } else {
            nearest = lower->intersect(ray, t_enter, t_split, next(dim));
            if (std::isinf(nearest.t) || nearest.t > t_split) {
                nearest = upper->intersect(ray, t_split, t_leave, next(dim));
            }
        }
        return nearest;
    }
    
    const size_t CUTOFF = 1;
    void split(short dim, int depth = 0) {
        if (objects.size() <= CUTOFF) {
            return;
        }
        double lower = INFINITY;
        double upper = -INFINITY;
        for (auto p : objects) {
            BBox b = p->bbox();
            lower = std::min(lower, b.lower.coord[dim]);
            upper = std::max(upper, b.upper.coord[dim]);
        }
        coord = (upper + lower) / 2;
        left = new Node();
        right = new Node();
        for (auto p : objects) {
            if (p->bbox().lower.coord[dim] <= coord) {
                left->objects.push_back(p);
            }
            if (p->bbox().upper.coord[dim] >= coord) {
                right->objects.push_back(p);
            }
        }

        if (depth < 10) {
            left->split(next(dim), depth + 1);
            right->split(next(dim), depth + 1);
        }
    }
};

class KDTree {
    BBox outer;
    Node* root;

public:
    void build(std::vector<Primitive*> objects) {
        root = new Node();
        root->objects = objects;
        outer = objects[0]->bbox();
        for (size_t i = 1; i < objects.size(); ++i) {
            outer += objects[i]->bbox();
        }
        root->split(0);
    }

    Intersection intersect(Ray ray) const {
        std::pair<double, double> inter = outer.intersect(ray);
        if (inter.second < 0 || almost_zero(inter.first - inter.second) || std::isinf(inter.second)) {
            return Intersection();
        }
        return root->intersect(ray, inter.first, inter.second, 0);
    }

    const BBox& get_bbox() {
        return outer;
    }
};
#endif
