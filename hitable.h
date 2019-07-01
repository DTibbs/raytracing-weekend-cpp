#pragma once

#include "material.h"
#include "ray.h"
#include "vec3.h"
#include <vector>

class Material;

class HitRecord {
public:
  HitRecord() {}
  HitRecord(float t, Vec3 p, Vec3 normal, Material* material) : m_t(t), m_p(p), m_normal(normal), m_material(material) {}
  virtual ~HitRecord() {}

  inline float t() const { return m_t; }
  inline Vec3 p() const { return m_p; }
  inline Vec3 normal() const { return m_normal; }
  inline Material* material() const { return m_material; }

protected:
  float m_t;
  Vec3 m_p;
  Vec3 m_normal;
  Material* m_material;
};

// Pure virtual abstract class
class Hitable {
public:

  // Classes that inherit Hitable must implement hit()
  // Using std::unique_ptr to mimic Rust's Option<> type without leaking memory by returning a pointer or NULL.
  virtual std::unique_ptr<HitRecord> hit(const Ray& r, const float t_min, const float t_max) = 0;
protected:
};

class HitableList: public Hitable {
public:
  // Potentially expensive copy of vector
  explicit HitableList(std::vector<Hitable*> list) : m_list(list) {}
  virtual ~HitableList() {}
  virtual std::unique_ptr<HitRecord> hit(const Ray& r, const float t_min, const float t_max);

protected:
  std::vector<Hitable*> m_list;
};