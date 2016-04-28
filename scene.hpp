#ifndef SCENE_H
#define SCENE_H
#include "primitive.hpp"
#include "ray.hpp"
#include "light.hpp"
#include "kdtree.hpp"
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
        return Ray(pos.start, direction);
    }

    std::pair<double, double> get_coord(Point p) const {
        Vector v = p - pos.start;
        double d = dot(v, pos.direction)/zoom;
        if (almost_zero(d)) {
            return {0, 0};
        }
        double x = dot(v, orientation)/d;
        double y = dot(v, vec(pos.direction, orientation))/d;
        return {x, y};
    }
};

class Scene {
public:
    std::vector<Primitive*> objects;
    std::vector<Light> lights;
    Camera camera;
    KDTree tree;
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

    void prepare() {
        tree.build(objects);
    }

    Ray get_ray(double x, double y) const {
        return camera.get_ray(x, y);
    }

    std::pair<double, double> get_coord(Point point) const {
        return camera.get_coord(point);
    }

    Intersection find_closest(Ray ray) const {
        return tree.intersect(ray);
    }

    const double AMBIENT = 0;
    Color trace_ray(Ray ray) const {
        Intersection closest = find_closest(ray);
        if (std::isinf(closest.t)) {
            return Color();
        }
        Point p = ray.get_point(closest.t);
        for (auto& light : lights) {
            Ray light_ray(p, light.point - p);
            double angle = dot(closest.object->normal(p), light_ray.direction);
            if (angle < 0) {
                continue;
            }
            Intersection obstacle = find_closest(light_ray);
            if (obstacle.t >= 1 - EPS) {
                return closest.object->texture(p)*(angle/sq(light_ray.direction) + AMBIENT);
            }
        }
        return closest.object->texture(p)*AMBIENT;
    }

    void find_best_view(double angle) {
        Vector dir = Vector(cos(angle), sin(angle), 0);
        const BBox& box = tree.get_bbox();
        Point center = (box.lower + box.upper) / 2;
        Vector radius = (box.upper - box.lower) / 2;
        double dist = std::max(radius.y, radius.z);
        center -= 5*dist*dir;
        camera = Camera(Ray(center, dir), Vector(0, 0, 1), 1);
        add_light(Light{center, Color()});
    }
};
#endif

