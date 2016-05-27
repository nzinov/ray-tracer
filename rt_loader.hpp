#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include <cassert>
#include "scene.hpp"
#include "triangle.hpp"
#include "quadrangle.hpp"
#include "sphere.hpp"
#include "material.hpp"
#include <map>

class RTLoader {
    std::ifstream file;
    std::map<std::string, Material*> dict;
public:
    template <typename T> bool get(T& val) {
        while (file && file.peek() != '-' && !isalnum(file.peek())) {
            if (file.peek() == '#') {
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                file.ignore();
            }
        }
        if (!file) {
            return false;
        }
        file >> val;
        return true;
    }

    void read_point(Point& p) {
        get(p.x);
        get(p.y);
        get(p.z);
    }

    Camera generate_camera(Point origin, Point tl, Point bl, Point tr) {
        Point center = (tr + bl) / 2;
        Point vert = center - (tr + tl) / 2;
        Point hor = center - (tl + bl) / 2;
        return Camera(Ray(origin, center - origin), hor, vert);
    }

    void expect(const std::string& expected) {
        std::string got;
        assert(get(got));
        if (expected != got) {
            printf("Expected '%s' got '%s'\n", expected.c_str(), got.c_str());
            exit(1);
        }
    }

    void populate(Scene& scene, const char* filename) {
        log("Begin loading");
        long long count = 0;
        file = std::ifstream(filename);
        std::string word;
        while (get(word)) {
            if (word == "triangle") {
                Triangle* obj = new Triangle();
                for (int i = 0; i < 3; ++i) {
                    expect("vertex");
                    read_point(obj->vertices[i]);
                }
                expect("material");
                get(word);
                obj->material = dict[word];
                scene.add_object(obj);
                obj->cache();
                ++count;
            } else if (word == "quadrangle") {
                Quadrangle* obj = new Quadrangle();
                for (int i = 0; i < 4; ++i) {
                    expect("vertex");
                    read_point(obj->vertices[i]);
                }
                expect("material");
                get(word);
                obj->material = dict[word];
                scene.add_object(obj);
                ++count;
            } else if (word == "sphere") {
                Sphere* obj = new Sphere();
                expect("coords");
                read_point(obj->center);
                expect("radius");
                get(obj->radius);
                expect("material");
                get(word);
                obj->material = dict[word];
                scene.add_object(obj);
                ++count;
            } else if (word == "entry") {
                Material* material = new Material();
                while (get(word) && word != "endentry") {
                    if (word == "name") {
                        get(word);
                        dict[word] = material;
                    } else if (word == "color") {
                        read_point(material->color);
                        material->color /= 255.0;
                    } else if (word == "reflect") {
                        get(material->reflectance);
                    } else if (word == "refract") {
                        get(material->index);
                    } else if (word == "alpha") {
                        get(material->alpha);
                    }
                }
            } else if (word == "viewport") {
                Point origin, tl, bl, tr;
                while (get(word) && word != "endviewport") {
                    if (word == "origin") {
                        read_point(origin);
                    } else if (word == "topleft") {
                        read_point(tl);
                    } else if (word == "bottomleft") {
                        read_point(bl);
                    } else if (word == "topright") {
                        read_point(tr);
                    }
                }
                scene.camera = generate_camera(origin, tl, bl, tr);
            } else if (word == "point") {
                Light light;
                expect("coords");
                read_point(light.point);
                double power;
                expect("power");
                get(power);
                light.color = Color(1, 1, 1) * power;
                scene.add_light(light);
            }
        }
        printf("Loaded %lli primitives\n", count);
        log("End loading");
        scene.prepare();
    }
};
