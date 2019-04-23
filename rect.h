#pragma once

#include "hitable.h"
#include "aabb.h"
#include "material.h"

class xy_rect final : public hitable
{
public:
    xy_rect(float x0, float x1, float y0, float y1, float k, std::unique_ptr<material> material)
    : x0_(x0)
    , x1_(x1)
    , y0_(y0)
    , y1_(y1)
    , k_(k)
    , material_(std::move(material))
    {}

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    bool bounding_box(float t0, float t1, aabb& box) const override
    {
        box = aabb(vec3(x0_, y0_, k_ - 0.0001), vec3(x1_, y1_, k_ + 0.0001));
        return true;
    }

private:
    float x0_, x1_, y0_, y1_, k_;
    std::unique_ptr<material> material_;
};

bool xy_rect::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    // Determine if the ray hits the plane
    float t = (k_ - r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max)
    {
        return false;
    }

    // Find the x, y coordinates of the ray/plane intersection
    float x = r.origin().x() + t * r.direction().x();
    float y = r.origin().y() + t * r.direction().y();
    if (x < x0_ || x > x1_ || y < y0_ || y > y1_)
    {
        return false;
    }

    rec.u = (x - x0_) / (x1_ - x0_);
    rec.v = (y - y0_) / (y1_ - y0_);
    rec.t = t;
    rec.mat_ptr = material_.get();
    rec.p = r.point_at_parameter(t);
    rec.normal = vec3(0, 0, 1);
    return true;
}
