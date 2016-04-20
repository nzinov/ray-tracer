#ifndef SCENE_H
#define SCENE_H
#include "primitive.hpp"
#include "ray.hpp"
#include <vector>
#include <memory>

class Camera {
    Ray pos;
    Vector orientation;
    double scale;
    double zoom;
public:
    Camera(Ray ray, Vector orientation, double scale, double zoom) : pos(ray), orientation(orientation), scale(scale), zoom(zoom) {}

    Ray get_ray(double x, double y) const {
        Vector ort = vec(pos.direction, orientation);
        Vector direction = pos.direction*zoom + orientation*x + ort*y;
        return Ray(pos.start, direction * scale);
    }
};

class Scene {
    std::vector<std::unique_ptr<Primitive> > objects;
    Camera camera;
public:
    Scene(Camera camera) : camera(camera) {}
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    ~Scene() {}

    void add_object(std::unique_ptr<Primitive> pointer) {
        objects.push_back(std::move(pointer));
    }

    Ray get_ray(double x, double y) const {
        return camera.get_ray(x, y);
    }

    Color trace_ray(Ray ray) const {
        Primitive* closest = nullptr;
        double min_dist = INFINITY;
        for (auto obj = objects.begin(); obj != objects.end(); ++obj) {
            Maybe<double> dist = (*obj)->intersect(ray);
            if (!dist) {
                continue;
            }
            if (dist.val() < min_dist) {
                min_dist = dist.val();
                closest = obj->get();
            }
        }
        if (!closest) {
            return Color();
        }
        return closest->texture(ray.get_point(min_dist));
    }
};
#endif

