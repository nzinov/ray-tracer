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
        short count = 1;
        while (file >> next) {
            if (next == "loop") {
                std::vector<Point> vertices(3);
                for (int i = 0; i < 3; ++i) {
                    file >> next >> vertices[i].x >> vertices[i].y >> vertices[i].z;
                }
                scene.add_object(new Triangle(Color(1, 1, 1), vertices[0], vertices[1], vertices[2]));
                ++count;
            }
        }
        scene.prepare();
    }
};
