#include <iostream>
#include <utility>

#include "camera.h"
#include "hitable_list.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

static const vec3 WHITE(1.0, 1.0, 1.0);
static const vec3 BLUE(0.5, 0.7, 1.0);

// Linearly interpolate between start and end. Assumes 0 <= t <= 1
vec3 lerp(float t, const vec3& start, const vec3& end)
{
    return (1.0 - t) * start + t * end;
}

// Generate a point in the unit sphere centered at the origin
vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        // generate point in unit cube centered at origin
        p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    }
    while (p.squared_length() >= 1.0); // reject if outside unit sphere
    return p;
}

// Color of the ray
vec3 color(const ray& r, hitable *world)
{
    hit_record rec;
    if (world->hit(r, 0.0, MAXFLOAT, rec))
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * color(ray(rec.p, target - rec.p), world);
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
    world.push_back(std::make_unique<sphere>(vec3(0, 0, -1), 0.5));
    world.push_back(std::make_unique<sphere>(vec3(0, -100.5, -1), 100));

    camera cam;
    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; ++s)
            {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);

                // vec3 p = r.point_at_parameter(2.0);
                col += color(r, &world);
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
