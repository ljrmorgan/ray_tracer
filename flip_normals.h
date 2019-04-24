#pragma once

#include <memory>

#include "hitable.h"

class flip_normals final : public hitable
{
public:
    flip_normals(std::unique_ptr<hitable> hitable)
    : hitable_(std::move(hitable))
    {}

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
    {
        if (hitable_->hit(r, t_min, t_max, rec))
        {
            rec.normal = -rec.normal;
            return true;
        }
        return false;
    }

    bool bounding_box(float t0, float t1, aabb& box) const override
    {
        return hitable_->bounding_box(t0, t1, box);
    }

private:
    std::unique_ptr<hitable> hitable_;
};
