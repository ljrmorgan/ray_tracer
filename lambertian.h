#pragma once

#include <memory>

#include "material.h"

#include "hitable.h"
#include "ray.h"
#include "texture.h"
#include "vec3.h"

class lambertian final : public material
{
public:
    lambertian(std::unique_ptr<texture> albedo)
    : albedo_(std::move(albedo))
    {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation,
        ray& scattered) const override
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p, r_in.time());
        attenuation = albedo_->value(0, 0, rec.p);
        return true;
    }

private:
    std::unique_ptr<texture> albedo_;
};
