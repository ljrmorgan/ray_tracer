#pragma once

#include "ray.h"

class camera
{
public:
    camera(float vfov, float aspect) // vfov: top to bottom and in degrees
    : origin_(0.0, 0.0, 0.0)
    {
        float theta = vfov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;

        lower_left_corner_ = vec3(-half_width, -half_height, -1.0);
        horizontal_ = vec3(2 * half_width, 0.0, 0.0);
        vertical_ = vec3(0.0, 2 * half_height, 0.0);
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
