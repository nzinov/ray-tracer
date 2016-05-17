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
    
    const size_t CUTOFF = 5;
    void split(short dim, int depth = 0) {
        if (objects.size() <= CUTOFF) {
            return;
        }
        std::sort(objects.begin(), objects.end(),
                [dim](Primitive* a, Primitive* b) { return a->bbox().upper.coord[dim] < b->bbox().upper.coord[dim]; });
        size_t split = objects.size() / 2;
        std::sort(objects.begin() + split, objects.end(),
                [dim](Primitive* a, Primitive* b) { return a->bbox().lower.coord[dim] < b->bbox().lower.coord[dim]; });
        coord = objects[split - 1]->bbox().upper.coord[dim];
        left = new Node();
        right = new Node();
        std::copy(objects.begin(), objects.begin() + split, std::back_inserter(left->objects));
        std::copy(objects.begin() + split, objects.end(), std::back_inserter(right->objects));
        for (auto iter = objects.begin() + split; iter != objects.end(); ++iter) {
            if ((*iter)->bbox().lower.coord[dim] > coord) {
                break;
            }
            left->objects.push_back(*iter);
        }
        if (depth < 10) {
            left->split(next(dim), depth + 1);
            right->split(next(dim), depth + 1);
        }
    }

    ~Node() {
        if (left) {
            delete left;
        }
        if (right) {
            delete right;
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
