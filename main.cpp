#include <iostream>
#include <utility>

#include "camera.h"
#include "hitable_list.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

static const vec3 WHITE(1.0, 1.0, 1.0);
static const vec3 BLUE(0.5, 0.7, 1.0);

// Linearly interpolate between start and end. Assumes 0 <= t <= 1
vec3 lerp(float t, const vec3& start, const vec3& end)
{
    return (1.0 - t) * start + t * end;
}

// Color of the ray
vec3 color(const ray& r, hitable *world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec))
    {
        ray scattered;
        vec3 attenutation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenutation, scattered))
        {
            return attenutation * color(scattered, world, depth + 1);
        }
        else
        {
            return vec3(0, 0, 0);
        }
    }

    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return lerp(t, WHITE, BLUE);
}

int main(int argc, char const *argv[])
{
    int nx = 200;
    int ny = 100;
    int ns = 100;

    std::cout << "P3" << std::endl
        << nx << " " << ny << std::endl
        << "255" << std::endl;

    hitable_list world;
    world.push_back(std::make_unique<sphere>(vec3(0, 0, -1), 0.5,
        std::make_unique<lambertian>(vec3(0.1, 0.2, 0.5))));
    world.push_back(std::make_unique<sphere>(vec3(0, -100.5, -1), 100,
        std::make_unique<lambertian>(vec3(0.8, 0.8, 0.0))));
    world.push_back(std::make_unique<sphere>(vec3(1, 0, -1), 0.5,
        std::make_unique<metal>(vec3(0.8, 0.6, 0.2), 0.6)));
    // Make a hollow glass sphere:
    world.push_back(std::make_unique<sphere>(vec3(-1, 0, -1), 0.5,
        std::make_unique<dielectric>(GLASS_REFRACTIVE_INDEX)));
    world.push_back(std::make_unique<sphere>(vec3(-1, 0, -1), -0.45,
        std::make_unique<dielectric>(GLASS_REFRACTIVE_INDEX)));

    camera cam(90, float(nx) / float(ny));
    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; ++s)
            {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);

                // vec3 p = r.point_at_parameter(2.0);
                col += color(r, &world, 0);
            }

            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            std::cout << ir << " " << ig << " " << ib << std::endl;
        }
    }
    return 0;
}
