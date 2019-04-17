#pragma once

#include "texture.h"

class constant_texture final : public texture
{
public:
    constant_texture(vec3 color)
    : color_(std::move(color))
    {}

    virtual vec3 value(float u, float v, const vec3& p) const override
    {
        return color_;
    }

private:
    vec3 color_;
};
