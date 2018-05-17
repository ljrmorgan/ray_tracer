#pragma once

#include "ray.h"

class camera
{
public:
    camera()
    : lower_left_corner_(-2.0, -1.0, -1.0)
    , horizontal_(4.0, 0.0, 0.0)
    , vertical_(0.0, 2.0, 0.0)
    , origin_(0.0, 0.0, 0.0)
    {}

    ray get_ray(float u, float v)
    {
        return ray(origin_, lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_);
    }

private:
    vec3 lower_left_corner_;
    vec3 horizontal_;
    vec3 vertical_;
    vec3 origin_;
};
