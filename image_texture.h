#pragma once

#include "texture.h"
#include "utils.h"

class image_texture final : public texture
{
public:
    using data_t = std::unique_ptr<unsigned char[]>;

    image_texture(data_t data, int nx, int ny)
    : data_(std::move(data))
    , nx_(nx)
    , ny_(ny)
    {}

    vec3 value(float u, float v, const vec3& p) const override
    {
        int i = clamp(u * nx_, 0, nx_ - 1);
        int j = clamp((1 - v) * ny_ - 0.001, 0, ny_ - 1);

        float r = int(data_[3 * i + 3 * nx_ * j]) / 255.0;
        float g = int(data_[3 * i + 3 * nx_ * j + 1]) / 255.0;
        float b = int(data_[3 * i + 3 * nx_ * j + 2]) / 255.0;
        return vec3(r, g, b);
    }
private:
    data_t data_;
    int nx_;
    int ny_;
};
