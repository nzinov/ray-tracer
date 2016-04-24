#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include "scene.hpp"
#include "triangle.hpp"

class STLLoader {
public:
    static void populate(Scene& scene, const char* filename) {
        std::ifstream file(filename);
        std::string next;
        while (file >> next) {
            if (next == "loop") {
                std::vector<Point> vertices(3);
                for (int i = 0; i < 3; ++i) {
                    file >> next >> vertices[i].x >> vertices[i].y >> vertices[i].z;
                }
                scene.add_object(std::make_unique<Triangle>(Color(0, 3, 0), vertices[0], vertices[1], vertices[2]));
            }
        }
    }
};
