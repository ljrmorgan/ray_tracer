#pragma once

#include "material.h"

#include "hitable.h"
#include "ray.h"
#include "vec3.h"

// Generate a point in the unit sphere centered at the origin
inline vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        // generate point in unit cube centered at origin
        p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    }
    while (p.squared_length() >= 1.0); // reject if outside unit sphere
    return p;
}

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
