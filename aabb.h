#pragma once

#include "vec3.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

/**
 * Axis Aligned Bounding Box
 */
class aabb final
{
public:
    aabb()
    {}

    aabb(vec3 a, vec3 b)
    : min_(std::move(a))
    , max_(std::move(b))
    {}

    const vec3& min() const
    {
        return min_;
    }

    const vec3& max() const
    {
        return max_;
    }

    bool hit(const ray& r, float tmin, float tmax) const
    {
        for (int a = 0; a < 3; ++a)
        {
            float invD = 1.0f / r.direction()[a];
            float t0 = (min()[a] - r.origin()[a]) * invD;
            float t1 = (max()[a] - r.origin()[a]) * invD;
            if (invD < 0.0f)
            {
                std::swap(t0, t1);
            }
            tmin = ffmax(t0, tmin);
            tmax = ffmin(t1, tmax);
            if (tmax <= tmin)
            {
                return false;
            }
        }
        return true;
    }

    static aabb surrounding_box(const aabb& first, const aabb& second)
    {
        vec3 min(
            ffmin(first.min_[0], second.min_[0]),
            ffmin(first.min_[1], second.min_[1]),
            ffmin(first.min_[2], second.min_[2]));
        vec3 max(
            ffmax(first.max_[0], second.max_[0]),
            ffmax(first.max_[1], second.max_[1]),
            ffmax(first.max_[2], second.max_[2]));
        return aabb(min, max);
    }

private:
    vec3 min_;
    vec3 max_;
};
