#pragma once

#include "hitable.h"
#include "ray.h"
#include "vec3.h"

class HitRecord;

// Abstract Material class
class Material {
public:
  virtual std::unique_ptr<Ray> scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation) = 0;
protected:
};

class Dielectric : public Material {
public: 
  explicit Dielectric(float ri) : m_ref_idx(ri) {}
  virtual ~Dielectric() {}

  // Dialectric's implementation of scatter
  virtual std::unique_ptr<Ray> scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation);

  // Util function for Dialectric
  static inline float schlick(float cosine, float ref_idx) {
    float r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 *= r0;
    return r0 + (1.0 - r0) * (1.0 - pow((1 - cosine), 5.0));
  }
protected:
  float m_ref_idx;
};

class Lambertian : public Material {
public:
  explicit Lambertian(const Vec3& a) : m_albedo(a) { }
  virtual ~Lambertian() {}

  // Lambertian's implementation of scatter
  virtual std::unique_ptr<Ray> scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation);

protected:
  Vec3 m_albedo;
};

class Metal : public Material {
public:
  explicit Metal(const Vec3& a, float f) : m_albedo(a), m_fuzz(f) {
    // clamp fuzz
    if (m_fuzz > 1.0) { m_fuzz = 1.0; }
  }

  // Metal's implementation of scatter
  virtual std::unique_ptr<Ray> scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation);
protected:
  Vec3 m_albedo;
  float m_fuzz;
};