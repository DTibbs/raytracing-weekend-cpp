#pragma once
#include "vec3.h"

class Ray {
public:
  Ray() {}
  Ray(const Vec3& origin, const Vec3& direction) : m_origin(origin), m_direction(direction) {}
  virtual ~Ray() {}

  // Accessors
  inline Vec3 origin() const { return this->m_origin; }
  inline Vec3 direction() const { return this->m_direction; }

  // LERP along a Ray
  inline Vec3 point_at_parameter(float f) const { return m_origin + f * m_direction; }

protected:
  Vec3 m_origin;
  Vec3 m_direction;
};