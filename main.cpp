#include <iostream>

#include "vec3.h"
#include "ray.h"

static const vec3 WHITE(1.0, 1.0, 1.0);
static const vec3 BLUE(0.5, 0.7, 1.0);
static const vec3 RED(1, 0, 0);

// Linearly interpolate between start and end. Assumes 0 <= t <= 1
vec3 lerp(float t, const vec3& start, const vec3& end)
{
    return (1.0 - t) * start + t * end;
}

bool hit_sphere(const vec3& center, float radius, const ray& r)
{
    // Equation of a sphere centered at C = (cx, cy, cz), radius R:
    // (x - cx)^2 + (y - cy)^2 + (z - cz)^2 = R^2
    // or in vector form:
    // dot((p - C), (p - C)) = R^2
    // In our case `p` is our ray, of the form:
    // p(t) = A + t * B
    // If our ray hits the sphere then there must be a `t` s.t:
    // dot((A + t * B - C), (A + t * B - C)) = R^2
    // Solving for t gives us:
    // 0 = dot(B, B) * t^2 + 2 * dot(B, A - C) * t + dot(A - C, A - C) - R^2
    // ray misses the spehere if no roots, touches the sphere if one root,
    // intersects twice if two roots
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

// Color of the ray
vec3 color(const ray& r)
{
    if (hit_sphere(vec3(0, 0, -1), 0.5, r))
    {
        return RED;
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

    // camera/origin in the middle of our plane
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            vec3 col = color(r);
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << std::endl;
        }
    }
    return 0;
}
