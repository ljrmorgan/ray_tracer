#pragma once

#include "material.h"

#include "ray.h"
#include "vec3.h"

static const float GLASS_REFRACTIVE_INDEX = 1.5; // somewhere in [1.3, 1.7]
static const float DIAMOND_REFRACTIVE_INDEX = 2.4;

class dielectric : public material
{
public:
    dielectric(float ref_idx)
    : ref_idx_(ref_idx)
    {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation,
        ray& scattered) const override
    {
        // absorb nothing
        attenuation = vec3(1.0, 1.0, 1.0);

        vec3 outward_normal;
        float ni_over_nt;
        if (dot(r_in.direction(), rec.normal) > 0)
        {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx_;
        }
        else
        {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx_;
        }

        vec3 refracted;
        // for now: always refract if possible
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
        {
            scattered = ray(rec.p, refracted);
        }
        else
        {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            scattered = ray(rec.p, reflected);
        }

        return true;
    }

private:
    float ref_idx_;
};
