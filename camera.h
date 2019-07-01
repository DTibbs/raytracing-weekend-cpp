#pragma once

#include "ray.h"
#include "vec3.h"

class Camera {
public:
  // Require origin, look_at, up, fov, and aspect to construct a Camera
  explicit Camera(Vec3 origin, Vec3 look_at, Vec3 up, float fov, float aspect);
  virtual ~Camera() {}

  Ray get_ray(float u, float v) const;

protected:

  Vec3 m_origin;
  Vec3 m_lower_left;
  Vec3 m_horizontal;
  Vec3 m_vertical;
};