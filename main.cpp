#define USE_BVH 1

#include <iostream>
#include <utility>

#include "vec3.h"
#include "utils.h"

#include "camera.h"
#include "ray.h"

#include "bvh.h"
#include "hitable_list.h"
#include "moving_sphere.h"
#include "sphere.h"

#include "dielectric.h"
#include "lambertian.h"
#include "metal.h"

#include "constant_texture.h"
#include "checkered_texture.h"

static const vec3 WHITE(1.0, 1.0, 1.0);
static const vec3 BLUE(0.5, 0.7, 1.0);

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
    return lerp(WHITE, BLUE, t);
}

hitable_list large_scene()
{
    hitable_list scene;

    // floor
    scene.push_back(std::make_unique<sphere>(vec3(0, -1000, 0), 1000,
        std::make_unique<lambertian>(
            std::make_unique<checkered_texture>(
                std::make_unique<constant_texture>(vec3(0.2, 0.3, 0.1)),
                std::make_unique<constant_texture>(vec3(0.9, 0.9, 0.9))))));

    // Lots of small random spheres
    float small_sphere_radius = 0.2;
    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9)
            {
                float choose_mat = drand48();
                if (choose_mat < 0.8) // pick diffuse
                {
                    vec3 albedo(drand48() * drand48(), drand48() * drand48(),
                        drand48() * drand48());
                    scene.push_back(std::make_unique<moving_sphere>(
                        center,
                        center + vec3(0, 0.5 * drand48(), 0),
                        0.0, // time0
                        1.0, // time1
                        small_sphere_radius,
                        std::make_unique<lambertian>(std::make_unique<constant_texture>(albedo))));
                }
                else if (choose_mat < 0.95) // pick metal
                {
                    vec3 albedo(0.5 * (1 + drand48()), 0.5 * (1 + drand48()),
                        0.5 * (1 + drand48()));
                    float fuzziness = 0.5 * drand48();
                    scene.push_back(std::make_unique<sphere>(center,
                        small_sphere_radius,
                        std::make_unique<metal>(albedo, fuzziness)));
                }
                else // pick glass
                {
                    scene.push_back(std::make_unique<sphere>(center,
                        small_sphere_radius,
                        std::make_unique<dielectric>(GLASS_REFRACTIVE_INDEX)));
                }
            }
        }
    }

    // brown diffuse sphere at the back
    scene.push_back(std::make_unique<sphere>(vec3(-4, 1, 0), 1.0,
        std::make_unique<lambertian>(std::make_unique<constant_texture>(vec3(0.4, 0.2, 0.1)))));
    // glass sphere in the middle
    scene.push_back(std::make_unique<sphere>(vec3(0, 1, 0), 1.0,
        std::make_unique<dielectric>(GLASS_REFRACTIVE_INDEX)));
    // metal sphere at the front
    scene.push_back(std::make_unique<sphere>(vec3(4, 1, 0), 1.0,
        std::make_unique<metal>(vec3(0.7, 0.6, 0.5), 0.0)));

    std::cerr << "Generated scene with " << scene.size() << " spheres"
        << std::endl;

    return scene;
}

camera large_camera(int nx, int ny)
{
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    float fov = 20;
    float dist_to_focus = 10.0;
    float aperture = 0.1;
    float time0 = 0.0;
    float time1 = 1.0;

    return camera(lookfrom, lookat, vup, fov, float(nx) / float(ny), aperture,
        dist_to_focus, time0, time1);
}

hitable_list small_scene()
{
    hitable_list scene;

    // floor
    scene.push_back(std::make_unique<sphere>(vec3(0, -100.5, -1), 100,
        std::make_unique<lambertian>(
            std::make_unique<constant_texture>(vec3(0.8, 0.8, 0.0)))));

    // sphere in the middle
    scene.push_back(std::make_unique<sphere>(vec3(0, 0, -1), 0.5,
        std::make_unique<lambertian>(
            std::make_unique<constant_texture>(vec3(0.1, 0.2, 0.5)))));

    // metal sphere on the right
    scene.push_back(std::make_unique<sphere>(vec3(1, 0, -1), 0.5,
        std::make_unique<metal>(vec3(0.8, 0.6, 0.2), 0.3)));

    // hollow glass sphere on the left
    scene.push_back(std::make_unique<sphere>(vec3(-1, 0, -1), 0.5,
        std::make_unique<dielectric>(GLASS_REFRACTIVE_INDEX)));
    scene.push_back(std::make_unique<sphere>(vec3(-1, 0, -1), -0.45,
        std::make_unique<dielectric>(GLASS_REFRACTIVE_INDEX)));

    return scene;
}

camera small_camera(int nx, int ny)
{ 
    vec3 lookfrom(-2, 2, 1);
    vec3 lookat(0, 0, -1);
    vec3 vup(0, 1, 0);
    float fov = 20;
    float dist_to_focus = 10.0;
    float aperture = 0.1;
    float time0 = 0.0;
    float time1 = 1.0;

    return camera(lookfrom, lookat, vup, fov, float(nx) / float(ny), aperture,
        dist_to_focus, time0, time1);
}

int main(int argc, char const *argv[])
{
    int nx = 1200;
    int ny = 800;
    int ns = 10;

#ifdef USE_BVH
    bvh_node world(large_scene().elements(), 0.0, 1.0);
#else
    hitable_list world = large_scene();
#endif

    camera cam = large_camera(nx, ny);

    std::cout << "P3" << std::endl
        << nx << " " << ny << std::endl
        << "255" << std::endl;

    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
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
