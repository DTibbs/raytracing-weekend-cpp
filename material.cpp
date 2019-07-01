#include "material.h"
#include "sphere.h"

std::unique_ptr<Ray> Dielectric::scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation) {
  Vec3 outward_normal;
  Vec3 reflected = r_in.direction().reflect(rec.normal());
  float ni_over_nt;
  attenuation = Vec3(1.0, 1.0, 1.0);
  float reflect_prob;
  float cosine;
  if(r_in.direction().dot(rec.normal()) > 0.0) {
    outward_normal = -rec.normal();
    ni_over_nt = m_ref_idx;
    cosine = m_ref_idx * r_in.direction().dot(rec.normal()) / r_in.direction().length();
  }
  else {
    outward_normal = rec.normal();
    ni_over_nt = 1.0 / m_ref_idx;
    cosine = -r_in.direction().dot(rec.normal()) / r_in.direction().length();
  }

  auto refracted = r_in.direction().refract(outward_normal, ni_over_nt);
  if(refracted != nullptr) {
    reflect_prob = Dielectric::schlick(cosine, m_ref_idx);
  }
  else {
    reflect_prob = 1.0;
  }

  if(FRAND() < reflect_prob) {
    return std::unique_ptr<Ray>{new Ray(rec.p(), reflected)};
  }
  else {
    return std::unique_ptr<Ray>{new Ray(rec.p(), *refracted)};
  }
}

std::unique_ptr<Ray> Lambertian::scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation) {
  Vec3 target = rec.p() + rec.normal() + Sphere::random_in_unit_sphere();
  attenuation = m_albedo;
  return std::unique_ptr<Ray>{new Ray(rec.p(), target - rec.p())};
}

std::unique_ptr<Ray> Metal::scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation) {
  Vec3 reflected = r_in.direction().unit_vector().reflect(rec.normal());
  Ray scattered(rec.p(), reflected + m_fuzz * Sphere::random_in_unit_sphere());
  attenuation = m_albedo;
  if(scattered.direction().dot(rec.normal()) > 0.0) {
    return std::unique_ptr<Ray>{new Ray(scattered)};
  }
  else {
    return nullptr;
  }
}