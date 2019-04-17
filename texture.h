#pragma once

#include "vec3.h"

class texture
{
public:
    virtual ~texture() {}
    virtual vec3 value(float u, float v, const vec3& p) const = 0;
};
