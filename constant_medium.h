#pragma once

#include "hitable.h"

#include <memory>

#include "texture.h"
#include "ray.h"
#include "vec3.h"
#include "aabb.h"
#include "isotropic.h"

class constant_medium final : public hitable
{
public:
    constant_medium(std::unique_ptr<hitable> boundary, float density, std::unique_ptr<texture> texture)
    : boundary_(std::move(boundary))
    , density_(density)
    , phase_function_(std::make_unique<isotropic>(std::move(texture)))
    {}

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
    {
        hit_record rec1, rec2;
        if (boundary_->hit(r, -FLT_MAX, FLT_MAX, rec1) && boundary_->hit(r, rec1.t + 0.0001, FLT_MAX, rec2))
        {
            if (rec1.t < t_min) rec1.t = t_min;
            if (rec2.t > t_max) rec2.t = t_max;
            if (rec1.t >= rec2.t) return false;
            if (rec1.t < 0) rec1.t = 0;

            float distance_inside_boundary = (rec2.t - rec1.t) * r.direction().length();
            float hit_distance = - (1 / density_) * log(drand48());
            rec.t = rec1.t + hit_distance / r.direction().length();
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = vec3(1, 0, 0);     // arbitrary
            rec.mat_ptr = phase_function_.get();
            return true;
        }
        return false;
    }

    bool bounding_box(float t0, float t1, aabb& box) const override
    {
        return boundary_->bounding_box(t0, t1, box);
    }

private:
    std::unique_ptr<hitable> boundary_;
    float density_;
    std::unique_ptr<material> phase_function_;
};
