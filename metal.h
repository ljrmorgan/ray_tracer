#pragma once

#include "material.h"

#include "ray.h"
#include "vec3.h"

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

class metal : public material
{
public:
    metal(vec3 albedo)
    : albedo_(std::move(albedo))
    {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation,
        ray& scattered) const override
    {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected);
        attenuation = albedo_;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    vec3 albedo_;
};
