#include <iostream>

#include "vec3.h"
#include "ray.h"

static const vec3 WHITE(1.0, 1.0, 1.0);
static const vec3 BLUE(0.5, 0.7, 1.0);

// Linearly interpolate between start and end. Assumes 0 <= t <= 1
vec3 lerp(float t, const vec3& start, const vec3& end)
{
    return (1.0 - t) * start + t * end;
}

float hit_sphere(const vec3& center, float radius, const ray& r)
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
    if (discriminant < 0)
    {
        return -1.0;
    }
    return (-b - sqrt(discriminant)) / (2.0 * a);
}

// Color of the ray
vec3 color(const ray& r)
{
    const vec3 sphere_center(0, 0, -1);
    float t = hit_sphere(sphere_center, 0.5, r);
    if (t > 0)
    {
        // Compute unit length normal (each component in [-1, 1])
        vec3 N = unit_vector(r.point_at_parameter(t) - sphere_center);
        // Map to vector where each component is in [0, 1]
        return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
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
