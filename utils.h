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
