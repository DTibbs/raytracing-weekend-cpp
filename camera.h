#pragma once

#include "ray.h"
#include "vec3.h"

class Camera {
public:
  // Require origin, look_at, up, fov, and aspect to construct a Camera
  explicit Camera(Vec3 origin, Vec3 look_at, Vec3 up, float fov, float aspect, float aperture, float focus_dist);
  virtual ~Camera() {}

  Ray get_ray(float u, float v) const;

  // Util function for getting a random point on X,Y plane
  static Vec3 random_in_unit_disk() {
    Vec3 p;
    do {
      p = 2.0 * Vec3(FRAND(), FRAND(), 0) - Vec3(1, 1, 0);
    } while(p.dot(p) >= 1.0);
    return p;
  }

protected:

  Vec3 m_origin;
  Vec3 m_lower_left;
  Vec3 m_horizontal;
  Vec3 m_vertical;
  Vec3 m_u;
  Vec3 m_v;
  Vec3 m_w;
  float m_lens_radius;
};