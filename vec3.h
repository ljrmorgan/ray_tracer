#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 {
public:
    vec3()
    : e_{}
    {}

    vec3(float e0, float e1, float e2)
    : e_{e0, e1, e2}
    {}

    inline float x() const { return e_[0]; }
    inline float y() const { return e_[1]; }
    inline float z() const { return e_[2]; }

    inline float r() const { return e_[0]; }
    inline float g() const { return e_[1]; }
    inline float b() const { return e_[2]; }

    inline const vec3& operator+() const { return *this; }
    inline vec3 operator-() const { return vec3{-e_[0], -e_[1], -e_[2]}; }
    inline float operator[](int i) const { return e_[i]; }
    inline float& operator[](int i) { return e_[i]; }

    inline vec3& operator+=(const vec3 &v2);
    inline vec3& operator-=(const vec3 &v2);
    inline vec3& operator*=(const vec3 &v2);
    inline vec3& operator/=(const vec3 &v2);
    inline vec3& operator*=(const float t);
    inline vec3& operator/=(const float t);

    inline float length() const {
        return sqrt(e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2]);
    }
    inline float squared_length() const {
        return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2];
    }
    inline void make_unit_vector();

private:
    float e_[3];
};

inline std::istream& operator>>(std::istream &is, vec3 &t) {
    return is >> t[0] >> t[1] >> t[2];
}

inline std::ostream& operator<<(std::ostream &os, const vec3 &t) {
    return os << t[0] << " " << t[1] << " " << t[2];
}

inline void vec3::make_unit_vector() {
    float k = 1.0 / length();
    e_[0] *= k;
    e_[1] *= k;
    e_[2] *= k;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return vec3{v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]};
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return vec3{v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]};
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
    return vec3{v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]};
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
    return vec3{v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2]};
}

inline vec3 operator*(float t, const vec3 &v) {
    return vec3{t * v[0], t * v[1], t * v[2]};
}

inline vec3 operator/(const vec3 &v, float t) {
    return vec3{v[0] / t, v[1] / t, v[2] / t};
}

inline vec3 operator*(const vec3 &v, float t) {
    return vec3{t * v[0], t * v[1], t * v[2]};
}

inline float dot(const vec3 &v1, const vec3 &v2) {
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec3{
        v1[1] * v2[2] - v1[2] * v2[1],
        -(v1[0] * v2[2] - v1[2] * v2[0]),
        v2[0] * v2[1] - v1[1] * v2[0]
    };
}

inline vec3& vec3::operator+=(const vec3 &v) {
    e_[0] += v.e_[0];
    e_[1] += v.e_[1];
    e_[2] += v.e_[2];
    return *this;
}

inline vec3& vec3::operator-=(const vec3 &v) {
    e_[0] -= v.e_[0];
    e_[1] -= v.e_[1];
    e_[2] -= v.e_[2];
    return *this;
}

inline vec3& vec3::operator*=(const vec3 &v) {
    e_[0] *= v.e_[0];
    e_[1] *= v.e_[1];
    e_[2] *= v.e_[2];
    return *this;
}

inline vec3& vec3::operator/=(const vec3 &v) {
    e_[0] /= v.e_[0];
    e_[1] /= v.e_[1];
    e_[2] /= v.e_[2];
    return *this;
}

inline vec3& vec3::operator*=(const float t) {
    e_[0] *= t;
    e_[1] *= t;
    e_[2] *= t;
    return *this;
}

inline vec3& vec3::operator/=(const float t) {
    float k = 1.0 / t;
    e_[0] *= k;
    e_[1] *= k;
    e_[2] *= k;
    return *this;
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

// Generate a point in the unit sphere centered at the origin
inline vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        // generate point in unit cube centered at origin
        p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    }
    while (p.squared_length() >= 1.0); // reject if outside unit sphere
    return p;
}

inline vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

inline bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0)
    {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    return false;
}
