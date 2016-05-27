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
        long long count = 0;
        Material* mat = new Material();
        mat->color = Color(1, 1, 1);
        while (file >> next) {
            if (next == "loop") {
                std::vector<Point> vertices(3);
                for (int i = 0; i < 3; ++i) {
                    file >> next >> vertices[i].coord[0] >> vertices[i].coord[1] >> vertices[i].coord[2];
                }
                Triangle* tri = new Triangle(vertices[0], vertices[1], vertices[2]);
                tri->material = mat;
                scene.add_object(tri);
                ++count;
            }
        }
        printf("%lli\n", count);
        log("End loading");
        scene.prepare();
    }
};
