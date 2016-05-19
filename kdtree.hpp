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
    short axis;
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

    Intersection intersect(const Ray& ray, double t_enter, double t_leave) const {
        if (is_leaf()) {
            return intersect_contents(ray);
        }
        double t_split = (coord - ray.start.coord[axis]) / ray.direction.coord[axis];
        Node* lower = left;
        Node* upper = right;
        Intersection nearest;
        if (ray.direction.coord[axis] < 0) {
            std::swap(lower, upper);
        }
        if (t_split < t_enter) {
            nearest = upper->intersect(ray, t_enter, t_leave);
        } else if (t_split > t_leave) {
            nearest = lower->intersect(ray, t_enter, t_leave);
        } else {
            nearest = lower->intersect(ray, t_enter, t_split);
            if (std::isinf(nearest.t) || nearest.t > t_split) {
                nearest = upper->intersect(ray, t_split, t_leave);
            }
        }
        return nearest;
    }
    
    void split(BBox box) {
        double best_value = INFINITY;
        BBox best_left_box;
        BBox best_right_box;
        std::vector<Primitive*> back_list(objects);
        for (int ax = 0; ax < 3; ++ax) {
            std::sort(objects.begin(), objects.end(),
                    [ax](Primitive* a, Primitive* b) { return a->bbox().lower.coord[ax] < b->bbox().lower.coord[ax]; });
            std::sort(back_list.begin(), back_list.end(),
                    [ax](Primitive* a, Primitive* b) { return a->bbox().upper.coord[ax] < b->bbox().upper.coord[ax]; });
            size_t left_count = 0;
            size_t right_count = objects.size();
            for (size_t i = 0; i < objects.size(); i += objects.size() / 100 + 1) {
                double splitter = back_list[i]->bbox().upper.coord[ax];
                while (left_count < objects.size() && objects[left_count]->bbox().lower.coord[ax] < splitter + EPS) {
                    ++left_count;
                }
                while (right_count > 0 && back_list[objects.size() - right_count]->bbox().upper.coord[ax] + EPS < splitter) {
                    --right_count;
                }
                if (splitter == box.lower.coord[ax]) {
                    continue;
                }
                BBox left_box = box;
                BBox right_box = box;
                left_box.upper.coord[ax] = splitter;
                right_box.lower.coord[ax] = splitter;
                double value = left_count * left_box.surface() + right_count * right_box.surface();
                if (value < best_value) {
                    axis = ax;
                    coord = splitter;
                    best_value = value;
                    best_left_box = left_box;
                    best_right_box = right_box;
                }
            }
        }
        if (best_value*1.5 + 1000 < objects.size() * box.surface()) {
            left = new Node();
            right = new Node();
            for (size_t i = 0; i < objects.size(); ++i) {
                if (objects[i]->bbox().upper.coord[axis] > coord - EPS) {
                    right->objects.push_back(objects[i]);
                }
                if (objects[i]->bbox().lower.coord[axis] < coord + EPS) {
                    left->objects.push_back(objects[i]);
                }
            }
            objects.clear();
            left->split(best_left_box);
            right->split(best_right_box);
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
        log("Begin building kd-tree");
        root = new Node();
        root->objects = objects;
        outer = objects[0]->bbox();
        for (size_t i = 1; i < objects.size(); ++i) {
            outer += objects[i]->bbox();
        }
        root->split(outer);
        log("End building kd-tree");
    }

    Intersection intersect(Ray ray) const {
        std::pair<double, double> inter = outer.intersect(ray);
        if (inter.second < 0 || almost_zero(inter.first - inter.second) || std::isinf(inter.second)) {
            return Intersection();
        }
        return root->intersect(ray, inter.first, inter.second);
    }

    const BBox& get_bbox() {
        return outer;
    }
};
#endif
