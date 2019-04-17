#pragma once

#include <memory>
#include <vector>

#include "hitable.h"
#include "ray.h"
#include "aabb.h"

class hitable_list final : public hitable
{
public:
    using size_type = std::vector<std::unique_ptr<hitable>>::size_type;

    hitable_list()
    {}

    void push_back(std::unique_ptr<hitable> &&element)
    {
        list_.push_back(std::move(element));
    }

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    virtual bool bounding_box(float t0, float t1, aabb &box) const override;

    size_type size() const
    {
        return list_.size();
    }

    // Let someone efficiently steal our guts if we're an rvalue reference
    std::vector<std::unique_ptr<hitable>> elements() &&
    {
        return std::move(list_);
    }

private:
    std::vector<std::unique_ptr<hitable>> list_;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (const auto &hitable : list_)
    {
        if (hitable->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool hitable_list::bounding_box(float t0, float t1, aabb &box) const
{
    if (list_.empty()) return false;
    aabb temp;
    if (!list_[0]->bounding_box(t0, t1, temp)) return false;
    box = temp;
    for (int i = 1; i < list_.size(); ++i)
    {
        if (!list_[i]->bounding_box(t0, t1, temp)) return false;
        box = aabb::surrounding_box(box, temp);
    }
    return true;
}
