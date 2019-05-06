#pragma once

#include "material.h"

#include <memory>
#include "texture.h"
#include "ray.h"
#include "hitable.h"
#include "vec3.h"

class isotropic final : public material
{
public:
    isotropic(std::unique_ptr<texture> albedo)
    : albedo_(std::move(albedo))
    {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation,
        ray& scattered) const override
    {
        scattered = ray(rec.p, random_in_unit_sphere(), rec.t);
        attenuation = albedo_->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    std::unique_ptr<texture> albedo_;
};
