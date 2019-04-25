#pragma once

#include "hitable.h"
#include "ray.h"
#include "material.h"
#include "utils.h"
#include "aabb.h"

class moving_sphere final : public hitable
{
public:
    moving_sphere(
        vec3 center0,
        vec3 center1,
        float time0,
        float time1,
        float radius,
        std::shared_ptr<material> material)
    : center0_(std::move(center0))
    , center1_(std::move(center1))
    , time0_(time0)
    , time1_(time1)
    , radius_(radius)
    , material_(std::move(material))
    {}

    ~moving_sphere()
    {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record &rec) const override;

    virtual bool bounding_box(float t0, float t1, aabb &box) const override
    {
        vec3 offset(radius_, radius_, radius_);
        aabb box0 = aabb(center0_ - offset, center0_ + offset);
        aabb box1 = aabb(center1_ - offset, center1_ + offset);
        box = aabb::surrounding_box(box0, box1);
        return true;
    }

    vec3 center(float time) const {
        float t = (time - time0_) / (time1_ - time0_);
        return lerp(center0_, center1_, t);
    }

private:
    vec3 center0_;
    vec3 center1_;
    float time0_;
    float time1_;
    float radius_;
    std::shared_ptr<material> material_;
};

bool moving_sphere::hit(const ray& r, float t_min, float t_max, hit_record &rec) const
{
    // Equation of a sphere centered at C = (cx, cy, cz), radius R:
    // (x - cx)^2 + (y - cy)^2 + (z - cz)^2 = R^2
    // or in vector form:
    // dot((p - C), (p - C)) = R^2
    // In our case `p` is our ray, of the form:
    // p(t) = A + t * B
    // If our ray hits the sphere then there must be a `t` s.t:
    // dot((A + t * B - C), (A + t * B - C)) = R^2
    // Solving for t gives us:
    // 0 = dot(B, B) * t^2 + 2 * dot(B, A - C) * t + dot(A - C, A - C) - R^2
    // ray misses the spehere if no roots, touches the sphere if one root,
    // intersects twice if two roots
    vec3 center_at_time = center(r.time());
    vec3 oc = r.origin() - center_at_time;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius_ * radius_;
    float discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center_at_time) / radius_;
            get_sphere_uv(rec.normal, rec.u, rec.v);
            rec.mat_ptr = material_.get();
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min)
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center_at_time) / radius_;
            get_sphere_uv(rec.normal, rec.u, rec.v);
            rec.mat_ptr = material_.get();
            return true;
        }
    }
    return false;
}
