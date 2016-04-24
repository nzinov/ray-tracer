#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include "scene.hpp"
#include "triangle.hpp"

class STLLoader {
public:
    static void populate(Scene& scene, const char* filename, int count) {
        std::ifstream file(filename);
        std::string next;
        while (file >> next) {
            if (next == "loop") {
                std::vector<Point> vertices(3);
                for (int i = 0; i < 3; ++i) {
                    file >> next >> vertices[i].x >> vertices[i].y >> vertices[i].z;
                    printf("(%f %f %f)", vertices[i].x, vertices[i].y, vertices[i].z);
                }
                printf("\n");
                scene.add_object(std::make_unique<Triangle>(Color(0, 1, 0), vertices[0], vertices[1], vertices[2]));
                count--;
                if (count == 0) {
                    break;
                }
            }
        }
    }
};
