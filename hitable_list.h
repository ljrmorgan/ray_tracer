#pragma once

#include <memory>
#include <vector>

#include "hitable.h"

class hitable_list : public hitable
{
public:
    hitable_list()
    {}

    void push_back(std::unique_ptr<hitable> &&element)
    {
        list_.push_back(std::move(element));
    }

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

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
