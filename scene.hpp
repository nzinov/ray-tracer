#ifndef SCENE_H
#define SCENE_H
#include "primitive.hpp"
#include "ray.hpp"
#include "light.hpp"
#include <vector>
#include <cmath>

class Camera {
    Ray pos;
    Vector orientation;
    double zoom;
public:
    Camera(Ray ray, Vector orientation, double zoom) : pos(ray), orientation(orientation), zoom(zoom) {
        orientation /= orientation.length();
    }

    Ray get_ray(double x, double y) const {
        Vector ort = vec(pos.direction, orientation);
        Vector direction = pos.direction*zoom + orientation*x + ort*y;
        return Ray(pos.start, direction / zoom);
    }
};

struct Intersection {
    double t;
    const Primitive& object;
};

class Scene {
    std::vector<Primitive*> objects;
    std::vector<Light> lights;
    Camera camera;
public:
    Scene(Camera camera) : camera(camera) {}
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    ~Scene() {
        for (auto el : objects) {
            delete el;
        }
    }

    void add_object(Primitive* pointer) {
        objects.push_back(pointer);
    }

    void add_light(Light light) {
        lights.push_back(light);
    }

    Ray get_ray(double x, double y) const {
        return camera.get_ray(x, y);
    }

    Intersection find_closest(Ray ray) const {
        const Primitive* closest;
        double min_dist = INFINITY;
        for (auto obj = objects.begin(); obj != objects.end(); ++obj) {
            double dist = (*obj)->intersect(ray);
            if (!std::isinf(dist) && dist > EPS && dist < min_dist) {
                min_dist = dist;
                closest = *obj;
            }
        }
        return {min_dist, *closest};
    }

    Color trace_ray(Ray ray) const {
        Intersection closest = find_closest(ray);
        if (std::isinf(closest.t)) {
            return Color(0.1, 0, 0);
        }
        Point p = ray.get_point(closest.t);
        for (auto& light : lights) {
            Ray light_ray(p, light.point - p);
            double angle = dot(closest.object.normal(p), light_ray.direction);
            if (angle < 0) {
                continue;
            }
            Intersection obstacle = find_closest(light_ray);
            if (obstacle.t >= 1 - EPS) {
                return closest.object.texture(p)*(angle/sq(light_ray.direction));
            }
        }
        return closest.object.texture(p)*0.1;
    }
};
#endif

