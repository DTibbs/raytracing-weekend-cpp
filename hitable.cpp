#include "hitable.h"

std::unique_ptr<HitRecord> HitableList::hit(const Ray& r, const float t_min, const float t_max) {
  auto rec = std::unique_ptr<HitRecord>(nullptr);
  double closest_so_far = t_max;
  // Find closest Hitable item in list based on given Ray and range t
  for(uint32_t i = 0; i < m_list.size(); ++i) {
    auto temp = m_list[i]->hit(r, t_min, closest_so_far);
    if(temp != nullptr) {
      // Update closest hit value
      closest_so_far = temp->t();
      
      // Release previous
      rec.release();
      
      // Move ownership of new one to result
      rec = std::move(temp);
    }
  }

  return rec;
}