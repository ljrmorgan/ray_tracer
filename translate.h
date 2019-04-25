#pragma once

#include "hitable.h"

class translate final : public hitable
{
public:
    translate(std::unique_ptr<hitable> hitable, vec3 offset)
    : hitable_(std::move(hitable))
    , offset_(std::move(offset))
    {}

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
    {
        ray moved_r(r.origin() - offset_, r.direction(), r.time());
        if (hitable_->hit(moved_r, t_min, t_max, rec))
        {
            rec.p += offset_;
            return true;
        }
        return false;
    }

    bool bounding_box(float t0, float t1, aabb& box) const override
    {
        if (hitable_->bounding_box(t0, t1, box))
        {
            box = aabb(box.min() + offset_, box.max() + offset_);
            return true;
        }

        return false;
    }

private:
    std::unique_ptr<hitable> hitable_;
    vec3 offset_;
};
