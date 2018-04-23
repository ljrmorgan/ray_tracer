#include <iostream>
#include "vec3.h"

int main(int argc, char const *argv[])
{
    int nx = 200;
    int ny = 100;
    std::cout << "P3" << std::endl
        << nx << " " << ny << std::endl
        << "255" << std::endl;
    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2);
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());
            std::cout << ir << " " << ig << " " << ib << std::endl;
        }
    }
    return 0;
}
