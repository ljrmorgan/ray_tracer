#pragma once

#include "material.h"

#include "hitable.h"
#include "ray.h"
#include "vec3.h"

class lambertian : public material
{
public:
    lambertian(vec3 albedo)
    : albedo_(std::move(albedo))
    {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation,
        ray& scattered) const override
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo_;
        return true;
    }

private:
    vec3 albedo_;
};
