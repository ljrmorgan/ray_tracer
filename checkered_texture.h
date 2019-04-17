#pragma once

#include <memory>

#include "texture.h"

class checkered_texture final : public texture
{
public:
    checkered_texture(std::unique_ptr<texture> odd, std::unique_ptr<texture> even)
    : odd_(std::move(odd))
    , even_(std::move(even))
    {}

    virtual vec3 value(float u, float v, const vec3& p) const override
    {
        float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        return sines < 0 ? odd_->value(u, v, p) : even_->value(u, v, p);
    }

private:
    std::unique_ptr<texture> odd_;
    std::unique_ptr<texture> even_;
};
