#define _USE_MATH_DEFINES

#include <cmath>
#include "camera.h"

Camera::Camera(Vec3 origin, Vec3 look_at, Vec3 up, float fov, float aspect)
  : m_origin(origin)
{
  float theta = fov * M_PI / 180.0;
  float half_height = tan(theta / 2.0);
  float half_width = aspect * half_height;
  Vec3 w = (origin - look_at).unit_vector();
  Vec3 u = up.cross(w).unit_vector();
  Vec3 v = w.cross(u);

  // Set the rest of the members after calculating
  m_lower_left = origin - half_width * u - half_height * v - w;
  m_horizontal = 2.0 * half_width * u;
  m_vertical = 2.0 * half_height * v;
}

Ray Camera::get_ray(float u, float v) const {
  return Ray(m_origin, m_lower_left + u * m_horizontal + v * m_vertical - m_origin);
}

