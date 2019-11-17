#define _USE_MATH_DEFINES

#include <cmath>
#include "camera.h"

Camera::Camera(Vec3 origin, Vec3 look_at, Vec3 up, float fov, float aspect, float aperture, float focus_dist)
  : m_origin(origin)
  , m_lens_radius(aperture / 2.0)
{
  float theta = fov * M_PI / 180.0;
  float half_height = tan(theta / 2.0);
  float half_width = aspect * half_height;
  
  // Set the rest of the members after calculating
  m_w = (origin - look_at).unit_vector();
  m_u = up.cross(m_w).unit_vector();
  m_v = m_w.cross(m_u);
  m_lower_left = origin - half_width * focus_dist * m_u - half_height * focus_dist * m_v - focus_dist * m_w;
  m_horizontal = 2.0 * half_width * focus_dist * m_u;
  m_vertical = 2.0 * half_height * focus_dist * m_v;
}

Ray Camera::get_ray(float u, float v) const {
  Vec3 rd = m_lens_radius * Camera::random_in_unit_disk();
  Vec3 offset = m_u * rd.x() + m_v * rd.y();
  // Use u,v passed in, not m_u, m_v;
  return Ray(m_origin + offset, m_lower_left + u * m_horizontal + v * m_vertical - m_origin - offset);
}

