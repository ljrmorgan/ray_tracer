#pragma once

#include "vec3.h"

// Linearly interpolate between start and end. Assumes 0 <= t <= 1
inline vec3 lerp(const vec3& start, const vec3& end, float t)
{
    return (1.0 - t) * start + t * end;
}

inline float lerp(float start, float end, float t)
{
    return (1.0 - t) * start + t * end;
}

// convert point of the surface of the unit sphere cenetered at the origin to texture coordinates
inline void get_sphere_uv(const vec3 &p, float &u, float &v)
{
    // convert to spherical coordinates
    float phi = atan2(p.z(), p.x());    // in range [-pi, pi]
    float theta = asin(p.y());          // in range [-pi / 2, pi / 2]

    // transform into [0, 1]
    u = 1 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI / 2) / M_PI;
}

inline float clamp(float value, float min, float max)
{
    if (value < min)
    {
        return min;
    }
    if (value > max)
    {
        return max;
    }
    return value;
}
