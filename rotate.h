#pragma once

#include "hitable.h"

#include <cfloat>       // FLT_MAX
#include <memory>       // unique_ptr

#include "ray.h"
#include "aabb.h"
#include "vec3.h"

class rotate_y final : public hitable
{
public:
    rotate_y(std::unique_ptr<hitable> hitable, float angle)
    : hitable_(std::move(hitable))
    {
        float radians = (M_PI / 180.0) * angle;
        sin_theta_ = sin(radians);
        cos_theta_ = cos(radians);
        has_box_ = hitable_->bounding_box(0, 1, box_);
        vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
        vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                for (int k = 0; k < 2; ++k)
                {
                    float x = i * box_.max().x() + (1 - i) * box_.min().x();
                    float y = j * box_.max().y() + (1 - j) * box_.min().y();
                    float z = k * box_.max().z() + (1 - k) * box_.min().z();
                    float new_x = cos_theta_ * x + sin_theta_ * z;
                    float new_z = -sin_theta_ * x + cos_theta_ * z;
                    vec3 tester(new_x, y, new_z);
                    for (int c = 0; c < 3; ++c)
                    {
                        if (tester[c] > max[c])
                        {
                            max[c] = tester[c];
                        }
                        if (tester[c] < min[c])
                        {
                            min[c] = tester[c];
                        }
                    }
                }
            }
        }
        box_ = aabb(min, max);
    }

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
    {
        vec3 origin(r.origin());
        origin[0] = cos_theta_ * r.origin()[0] - sin_theta_ * r.origin()[2];
        origin[2] = sin_theta_ * r.origin()[0] + cos_theta_ * r.origin()[2];

        vec3 direction(r.direction());
        direction[0] = cos_theta_ * r.direction()[0] - sin_theta_ * r.direction()[2];
        direction[2] = sin_theta_ * r.direction()[0] + cos_theta_ * r.direction()[2];

        ray rotated_r(origin, direction, r.time());

        if (hitable_->hit(rotated_r, t_min, t_max, rec))
        {
            vec3 p(rec.p);
            p[0] = cos_theta_ * rec.p[0] + sin_theta_ * rec.p[2];
            p[2] = -sin_theta_ * rec.p[0] + cos_theta_ * rec.p[2];

            vec3 normal(rec.normal);
            normal[0] = cos_theta_ * rec.normal[0] + sin_theta_ * rec.normal[2];
            normal[2] = -sin_theta_ * rec.normal[0] + cos_theta_ * rec.normal[2];

            rec.p = p;
            rec.normal = normal;
            return true;
        }
        return false;
    }

    bool bounding_box(float t0, float t1, aabb& box) const override
    {
        box = box_;
        return has_box_;
    }

private:
    std::unique_ptr<hitable> hitable_;
    float sin_theta_;
    float cos_theta_;
    bool has_box_;
    aabb box_;
};
