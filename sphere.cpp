#include "sphere.h"

std::unique_ptr<HitRecord> Sphere::hit(const Ray& r, const float t_min, const float t_max) {
  Vec3 oc = r.origin() - m_center;
  float a = r.direction().dot(r.direction());
  float b = oc.dot(r.direction());
  float c = oc.dot(oc) - m_radius * m_radius;
  float discriminant = b * b - a * c;
  if (discriminant > 0) {
    float temp = (-b - sqrt(discriminant)) / a;
    if (temp < t_max && temp > t_min) {
      Vec3 point(r.point_at_parameter(temp));
      return std::unique_ptr<HitRecord>{new HitRecord(temp, point, ((point - m_center) / m_radius), m_material)};
    }
    else {
      temp = (-b + sqrt(discriminant)) / a;
      if (temp < t_max && temp > t_min) {
        Vec3 point(r.point_at_parameter(temp));
        return std::unique_ptr<HitRecord>{new HitRecord(temp, point, ((point - m_center) / m_radius), m_material)};
      }
    }
  }

  return nullptr;
}