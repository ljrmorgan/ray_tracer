#pragma once

#include "vec3.h"

class ray
{
public:
    ray()
    {}

    ray(vec3 a, vec3 b, float time)
    : a_(std::move(a))
    , b_(std::move(b))
    , time_(time)
    {}

    const vec3 &origin() const
    {
        return a_;
    }

    const vec3 &direction() const
    {
        return b_;
    }

    float time() const
    {
        return time_;
    }

    vec3 point_at_parameter(float t) const
    {
        return a_ + t * b_;
    }

private:
    vec3 a_;
    vec3 b_;
    float time_;
};
