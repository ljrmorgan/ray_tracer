#pragma once

#include "vec3.h"

class ray
{
public:
    ray()
    {}

    ray(vec3 a, vec3 b)
    : a_(std::move(a))
    , b_(std::move(b))
    {}

    const vec3 &origin() const
    {
        return a_;
    }

    const vec3 &direction() const
    {
        return b_;
    }

    vec3 point_at_parameter(float t) const
    {
        return a_ + t * b_;
    }

private:
    vec3 a_;
    vec3 b_;
};
