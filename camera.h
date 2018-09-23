#pragma once

#include "ray.h"
#include "vec3.h"

class camera
{
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) // vfov: top to bottom and in degrees
    {
        float theta = vfov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;

        vec3 w = unit_vector(lookfrom - lookat);
        vec3 u = unit_vector(cross(vup, w));
        vec3 v = cross(w, u);

        origin_ = lookfrom;
        lower_left_corner_ = origin_ - half_width * u - half_height * v - w;
        horizontal_ = 2 * half_width * u;
        vertical_ = 2 * half_height * v;
    }

    ray get_ray(float u, float v)
    {
        return ray(origin_, lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_);
    }

private:
    vec3 origin_;
    vec3 lower_left_corner_;
    vec3 horizontal_;
    vec3 vertical_;
};
