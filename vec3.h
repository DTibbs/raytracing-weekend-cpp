#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

#define FRAND() (double(rand()) / (double(RAND_MAX) + 1.0))

class Vec3 {
public:
  Vec3():m_e{0} {}
  Vec3(float e0, float e1, float e2) : m_e{ e0, e1, e2 } {}
  virtual ~Vec3() {}

  // Treat Vec3 as x,y,z, accessors
  inline float x() const { return m_e[0]; }
  inline float y() const { return m_e[1]; }
  inline float z() const { return m_e[2]; }

  // Treat Vec3 as r,g,b, accessors
  inline float r() const { return m_e[0]; }
  inline float g() const { return m_e[1]; }
  inline float b() const { return m_e[2]; }

  // Array[] accessors
  inline float operator[](int i) const { return m_e[i]; }
  inline float& operator[](int i) { return m_e[i]; }

  // Pos/neg operators
  inline const Vec3& operator+() const { return *this; }
  inline Vec3 operator-() const { return Vec3(-m_e[0], -m_e[1], -m_e[2]); }

  inline float dot(const Vec3& v) const {
    return (this->x() * v.x() + this->y() * v.y() + this->z() * v.z());
  }

  // Right-hand rule for Cross product of two vectors in 3D space
  inline Vec3 cross(const Vec3& v) const {
    return Vec3(
        (this->y() * v.z()) - (this->z() * v.y()), 
        (this->z() * v.x()) - (this->x() * v.z()),
        (this->x() * v.y()) - (this->y() * v.x())
    );
  }

  // Reflect based on surface normal provided
  inline Vec3 reflect(const Vec3& n) const{
    return *this - 2 * dot(n) * n;
  }

  // Snell's law for refraction
  inline std::unique_ptr<Vec3> refract(Vec3 n, float ni_over_nt) {
    Vec3 uv = this->unit_vector();
    float dt = uv.dot(n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);
    if(discriminant > 0) {
      return std::unique_ptr<Vec3>{new Vec3(ni_over_nt * (uv - n * dt) - n * sqrt(discriminant))};
    }
    else {
      return nullptr;
    }
  }

  inline float length() const {
    return sqrt(x() * x() + y() * y() + z() * z());
  }

  inline float squared_length() const {
    return x() * x() + y() * y() + z() * z();
  }

  // Return a normalized copy of Vec3
  inline Vec3 unit_vector() const {
    return *this / this->length();
  }

  // Normalize Vec3 in-place
  inline void make_unit_vector() {
    float l = 1.0 / this->length();
    this->m_e[0] *= l;
    this->m_e[1] *= l;
    this->m_e[2] *= l;
  }

  // Math functions on Vec3s
  inline Vec3& operator+=(const Vec3& v) {
    m_e[0] += v.m_e[0];
    m_e[1] += v.m_e[1];
    m_e[2] += v.m_e[2];
    return *this;
  }

  inline Vec3& operator-=(const Vec3& v) {
    m_e[0] -= v.m_e[0];
    m_e[1] -= v.m_e[1];
    m_e[2] -= v.m_e[2];
    return *this;
  }

  inline Vec3& operator*=(const Vec3& v) {
    m_e[0] *= v.m_e[0];
    m_e[1] *= v.m_e[1];
    m_e[2] *= v.m_e[2];
    return *this;
  }

  inline Vec3& operator/=(const Vec3& v) {
    m_e[0] /= v.m_e[0];
    m_e[1] /= v.m_e[1];
    m_e[2] /= v.m_e[2];
    return *this;
  }

  // Math functions for Vec3 and floats
  inline Vec3& operator*=(const float f) {
    m_e[0] *= f;
    m_e[1] *= f;
    m_e[2] *= f;
    return *this;
  }

  inline Vec3& operator/=(const float f) {
    m_e[0] /= f;
    m_e[1] /= f;
    m_e[2] /= f;
    return *this;
  }

  // Friend the global operators or incur the wrath of C2676 and C2677
  friend Vec3 operator+(const Vec3& vl, const Vec3& vr);
  friend Vec3 operator-(const Vec3& vl, const Vec3& vr);
  friend Vec3 operator*(const Vec3& vl, const Vec3& vr);
  friend Vec3 operator/(const Vec3& vl, const Vec3& vr);
  friend Vec3 operator*(const Vec3& v, float f);
  friend Vec3 operator*(float f, const Vec3& v);
  friend Vec3 operator/(const Vec3& v, float f);
  friend Vec3 operator/(float f, const Vec3& v);

protected:
  // x,y,z or r,g,b
  float m_e[3];
};

// Operators for printing out a Vec3
inline std::istream& operator>>(std::istream& is, Vec3& v) {
  is >> v[0] >> v[1] >> v[2];
  return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& v) {
  os << v[0] << " " << v[1] << " " << v[2];
  return os;
}


// Global Math operators for Vec3 w/ other Vec3s
inline Vec3 operator+(const Vec3& vl, const Vec3& vr) {
  return Vec3(vl[0] + vr[0],
              vl[1] + vr[1],
              vl[2] + vr[2]);
}

inline Vec3 operator-(const Vec3& vl, const Vec3& vr) {
  return Vec3(vl[0] - vr[0],
              vl[1] - vr[1],
              vl[2] - vr[2]);
}

inline Vec3 operator*(const Vec3& vl, const Vec3& vr) {
  return Vec3(vl[0] * vr[0],
              vl[1] * vr[1],
              vl[2] * vr[2]);
}

inline Vec3 operator/(const Vec3& vl, const Vec3& vr) {
  return Vec3(vl[0] / vr[0],
              vl[1] / vr[1],
              vl[2] / vr[2]);
}

// Global Math operators for floats w/ Vec3s
inline Vec3 operator*(const Vec3& v, float f) {
  return Vec3(v[0] * f,
              v[1] * f,
              v[2] * f);
}

inline Vec3 operator*(float f, const Vec3& v) {
  return Vec3(f * v[0],
              f * v[1],
              f * v[2]);
}

inline Vec3 operator/(const Vec3& v, float f) {
  return Vec3(v[0] / f,
              v[1] / f,
              v[2] / f);
}

inline Vec3 operator/(float f, const Vec3& v) {
  return Vec3(f / v[0],
              f / v[1],
              f / v[2]);
}
