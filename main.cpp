#include <iostream>

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

// Color of the ray
vec3 color(const ray& r, hitable *world)
{
    hit_record rec;
    if (world->hit(r, 0.0, MAXFLOAT, rec))
    {
        vec3 &N = rec.normal;
        // Map to vector where each component is in [0, 1]
        return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return lerp(t, WHITE, BLUE);
}

int main(int argc, char const *argv[])
{
    int nx = 200;
    int ny = 100;
    std::cout << "P3" << std::endl
        << nx << " " << ny << std::endl
        << "255" << std::endl;

    hitable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitable_list(list, 2);

    camera cam;
    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray r = cam.get_ray(u, v);

            // vec3 p = r.point_at_parameter(2.0);
            vec3 col = color(r, world);
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            std::cout << ir << " " << ig << " " << ib << std::endl;
        }
    }
    return 0;
}
