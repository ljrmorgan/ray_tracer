#pragma once

#include "material.h"

#include "ray.h"
#include "vec3.h"

static const float GLASS_REFRACTIVE_INDEX = 1.5; // somewhere in [1.3, 1.7]
static const float DIAMOND_REFRACTIVE_INDEX = 2.4;

inline float schlick(float cosine, float ref_idx)
{
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class dielectric final : public material
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
        float cosine;
        if (dot(r_in.direction(), rec.normal) > 0)
        {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx_;
            cosine = ref_idx_ * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        else
        {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx_;
            cosine = - dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }

        vec3 refracted;
        float reflect_prob;
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
        {
            reflect_prob = schlick(cosine, ref_idx_);
        }
        else
        {
            reflect_prob = 1.0;
        }

        if (drand48() < reflect_prob)
        {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            scattered = ray(rec.p, reflected, r_in.time());
        }
        else
        {
            scattered = ray(rec.p, refracted, r_in.time());
        }

        return true;
    }

private:
    float ref_idx_;
};
