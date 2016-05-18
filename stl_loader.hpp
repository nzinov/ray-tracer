#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include "scene.hpp"
#include "triangle.hpp"
#include "material.hpp"

class STLLoader {
public:
    static void populate(Scene& scene, const char* filename) {
        log("Begin loading");
        std::ifstream file(filename);
        std::string next;
        short count = 1;
        Material* mat = new Material();
        mat->color = Color(1, 1, 1);
        while (file >> next) {
            if (next == "loop") {
                std::vector<Point> vertices(3);
                for (int i = 0; i < 3; ++i) {
                    file >> next >> vertices[i].x >> vertices[i].y >> vertices[i].z;
                }
                Triangle* tri = new Triangle(vertices[0], vertices[1], vertices[2]);
                tri->material = mat;
                scene.add_object(tri);
                ++count;
            }
        }
        log("End loading");
        scene.prepare();
    }
};
