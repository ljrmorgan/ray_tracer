#pragma once

class ray;
struct hit_record;
class vec3;

class material
{
public:
    virtual ~material()
    {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation,
        ray& scattered) const = 0;
};
