#pragma once

#include "hitable.h"
#include "aabb.h"
#include "hitable_list.h"
#include "rect.h"
#include "vec3.h"

class box final : public hitable
{
public:
    box(vec3 p0, vec3 p1, std::shared_ptr<material> material)
    {
        list_.push_back(
            std::make_unique<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), material));
        list_.push_back(std::make_unique<flip_normals>(
            std::make_unique<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material)));
        list_.push_back(
            std::make_unique<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), material));
        list_.push_back(std::make_unique<flip_normals>(
            std::make_unique<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), material)));
        list_.push_back(
            std::make_unique<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), material));
        list_.push_back(std::make_unique<flip_normals>(
            std::make_unique<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), material)));
        pMin_ = std::move(p0);
        pMax_ = std::move(p1);
    }

    bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override
    {
        return list_.hit(r, t_min, t_max, rec);
    }

    bool bounding_box(float t0, float t1, aabb& box) const override
    {
        box = aabb(pMin_, pMax_);
        return true;
    }

private:
    vec3 pMin_;
    vec3 pMax_;
    hitable_list list_;
};
