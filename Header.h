#pragma once

#include "vec3.h"

class Camera {
public:
  // Require origin, look_at, up, fov, and aspect to construct a Camera
  explicit Camera(Vec3 origin, Vec3 look_at, Vec3 up, float fov, float aspect);
  virtual ~Camera();

protected:

  Vec3 m_origin;
  Vec3 m_lower_left;
  Vec3 m_horizontal;
  Vec3 m_vertical;
};