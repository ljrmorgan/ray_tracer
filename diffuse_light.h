#pragma once

#include <memory>

#include "material.h"
#include "texture.h"

class diffuse_light final : public material
{
public:
    diffuse_light(std::unique_ptr<texture> emit)
    : emit_(std::move(emit))
    {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation,
        ray& scattered) const override
    {
        return false;
    }

    vec3 emitted(float u, float v, const vec3& p) const override
    {
        return emit_->value(u, v, p);
    }

private:
    std::unique_ptr<texture> emit_;
};
