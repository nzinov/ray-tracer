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
        if (depth >= 3) {
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
            double ang = fabs(angle(normal, light_ray.direction));
            Intersection obstacle = find_closest(light_ray);
            if (obstacle.t <= EPS || obstacle.t >= 1 - EPS) {
                double dist = light_ray.direction.length();
                result += (light.color * texture.color) * ang * (1 - texture.reflectance) / sq(dist);
            }
        }
        if (!almost_zero(texture.reflectance)) {
            Vector refl = normal*dot(normal, ray.direction)*(-2) + ray.direction;
            result += trace_ray(Ray(p, refl), depth + 1) * texture.reflectance;
        }
        if (!almost_zero(texture.index)) {
            double eta = 1.0 / texture.index;
            double cos_theta = -dot(normal, ray.direction);
            if(cos_theta < 0)
            {
                cos_theta *= -1.0;
                normal *= -1.0;
                eta = 1.0 / eta;
            }
            float k = 1.0 - sq(eta) * (1.0 - sq(cos_theta));
            if (k >= 0.0) {
                Vector refract = eta * ray.direction + (eta * cos_theta - sqrt(k)) * normal;
                result += trace_ray(Ray(p, refract), depth + 1);
            }
        }
        return result;
    }

    void find_best_view(double angle, bool light = true) {
        Vector dir = Vector(cos(angle), sin(angle), 0);
        const BBox& box = tree.get_bbox();
        Point center = (box.lower + box.upper) / 2;
        Vector radius = (box.upper - box.lower) / 2;
        double dist = std::max(radius.y, radius.z);
        center -= 5*dist*dir;
        camera = Camera(Ray(center, dir), Vector(0, 0, 1), vec(dir, Vector(0, 0, 1)));
        if (light) {
            add_light(Light{center, Color(1, 1, 1)});
        }
    }

    friend class RTLoader;
};
#endif

