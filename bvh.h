#pragma once

#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <vector>

#include "hitable.h"
#include "aabb.h"

using hitable_ptr = std::unique_ptr<hitable>;
using hitable_ptrs = std::vector<hitable_ptr>;

/**
 * Bounded Volume Hierarchy
 */
class bvh_node final : public hitable
{
public:
    bvh_node(hitable_ptrs l, float t0, float t1);

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
    virtual bool bounding_box(float t0, float t1, aabb &box) const override;

private:
    hitable_ptr left_;
    hitable_ptr right_;        // could be null
    aabb box_;
};

static bool box_comparator(int axis, float t0, float t1, const hitable_ptr &a, const hitable_ptr &b)
{
    aabb box_left, box_right;
    if (!a->bounding_box(t0, t1, box_left) || !b->bounding_box(t0, t1, box_right))
    {
        assert(false && "Missing bounding box!");
    }
    return box_left.min()[0] - box_right.min()[0] < 0.0;
}

bvh_node::bvh_node(hitable_ptrs l, float t0, float t1)
: left_()
, right_()
, box_()
{
    int axis = int(3 * drand48());
    std::sort(l.begin(), l.end(), [axis, t0, t1](const hitable_ptr &a, const hitable_ptr &b)
    {
        return box_comparator(axis, t0, t1, a, b);
    });
    if (l.size() == 1)
    {
        left_ = std::move(l[0]);
    }
    else if (l.size() == 2)
    {
        left_ = std::move(l[0]);
        right_ = std::move(l[1]);
    }
    else
    {
        // split l in two: first half stays in l, second half moves to r
        hitable_ptrs::iterator midpoint = l.begin() + l.size() / 2;
        hitable_ptrs r(std::make_move_iterator(midpoint),
            std::make_move_iterator(l.end()));
        l.erase(midpoint, l.end());
        left_ = std::make_unique<bvh_node>(std::move(l), t0, t1);
        right_ = std::make_unique<bvh_node>(std::move(r), t0, t1);
    }
    aabb box_left, box_right;
    if (!left_->bounding_box(t0, t1, box_left) || (right_ && !right_->bounding_box(t0, t1, box_right)))
    {
        assert(false && "Missing bounding box!");
    }
    box_ = aabb::surrounding_box(box_left, box_right);
}

bool bvh_node::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    if (box_.hit(r, t_min, t_max))
    {
        hit_record left_rec, right_rec;
        bool hit_left = left_->hit(r, t_min, t_max, left_rec);
        bool hit_right = right_ && right_->hit(r, t_min, t_max, right_rec);
        if (hit_left && hit_right)
        {
            if (left_rec.t < right_rec.t)
            {
                rec = left_rec;
            }
            else
            {
                rec = right_rec;
            }
            return true;
        }
        if (hit_left)
        {
            rec = left_rec;
            return true;
        }
        if (hit_right)
        {
            rec = right_rec;
            return true;
        }
    }
    return false;
}

bool bvh_node::bounding_box(float t0, float t1, aabb &box) const
{
    box = box_;
    return true;
}
