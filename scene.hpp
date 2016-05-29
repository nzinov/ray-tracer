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
    Vector vert;
    Vector hor;
public:
    Camera() {}
    Camera(Ray ray, Vector vert, Vector hor) : pos(ray), vert(vert), hor(hor) {}

    Ray get_ray(double x, double y) const {
        Vector direction = pos.direction + hor*x + vert*y;
        return Ray(pos.start, direction);
    }

    std::pair<double, double> get_coord(Point p) const {
        Vector v = p - pos.start;
        double d = dot(v, pos.direction);
        if (almost_zero(d)) {
            return {0, 0};
        }
        double x = dot(v, hor) / d;
        double y = dot(v, vert) / d;
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
    Scene() {}
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

    const Color AMBIENT = Color(0.1, 0.1, 0.1);

    Color trace_ray(Ray ray, short depth = 0) const {
        if (depth > 2) {
            return Color();
        }
        Intersection closest = find_closest(ray);
        if (std::isinf(closest.t)) {
            return Color();
        }
        Point p = ray.get_point(closest.t);
        Color result;
        const Material& texture = closest.object->texture(p);
        Vector normal = closest.object->normal(p);
        for (auto& light : lights) {
            Ray light_ray(p, light.point - p);
            Intersection obstacle = find_closest(light_ray);
            if (obstacle.t <= EPS || obstacle.t >= 1 - EPS) {
                result += texture.color;
            }
        }
        return result;
    }


    friend class RTLoader;
};
#endif

