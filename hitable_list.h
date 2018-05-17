#pragma once

#include "hitable.h"

class hitable_list : public hitable
{
public:
    hitable_list()
    {}

    hitable_list(hitable **list, int list_size)
    : list_(list)
    , list_size_(list_size)
    {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

private:
    hitable **list_;
    int list_size_;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for (int i = 0; i < list_size_; ++i)
    {
        if (list_[i]->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}
