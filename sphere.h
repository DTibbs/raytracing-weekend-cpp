#pragma once

#include "hitable.h"
#include "vec3.h"
#include "material.h"


class Sphere: public Hitable {
public:
  Sphere() {}
  Sphere(Vec3 center, float radius, Material* material) : m_center(center), m_radius(radius), m_material(material) {}
  virtual ~Sphere() {}

  virtual std::unique_ptr<HitRecord> hit(const Ray& r, const float t_min, const float t_max);

  // Util function of Sphere class
  static Vec3 random_in_unit_sphere() {
    Vec3 p;
    
    // Loop until we randomly generate a point inside a unit sphere (radius == 1.0)
    do {
      p = 2.0 * Vec3(FRAND(), FRAND(), FRAND()) - Vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
  }

protected:
  Vec3 m_center;
  float m_radius;
  Material* m_material;
};