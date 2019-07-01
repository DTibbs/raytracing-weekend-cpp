#include <windows.h>
#include <ppl.h>

#include <array>
#include <float.h>
#include <iostream>
#include <stdint.h>
#include <vector>
#include "camera.h"
#include "hitable.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

static const uint32_t RES_X = 1024;
static const uint32_t RES_Y = 768;
// Number of samples for anti-aliasing
static const uint32_t NUM_SAMPLES = 20;

Vec3 color(const Ray& r, Hitable* world, int depth) {
  auto rec = world->hit(r, 0.001, FLT_MAX);
  if(rec != nullptr) {
    Vec3 attenuation;
    auto scattered = rec->material()->scatter(r, *rec, attenuation);
    if(depth < 50 && scattered != nullptr) {
      return attenuation * color(*scattered, world, depth + 1);
    }
    else {
      return Vec3(0, 0, 0);
    }
  }
  else {
    Vec3 unit_direction = r.direction().unit_vector();
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
  }
}

void generate_ppm(Hitable* world, const Camera& cam) {
  for(int j = RES_Y - 1; j >= 0; --j) {
    for(int i = 0; i < RES_X; ++i) {
      Vec3 col(0.0, 0.0, 0.0);
      // Generate colors at random near current X,Y
      for(int s = 0; s < NUM_SAMPLES; ++s) {
        float u = float(i + FRAND()) / float(RES_X);
        float v = float(j + FRAND()) / float(RES_Y);
        Ray r = cam.get_ray(u, v);
        Vec3 p = r.point_at_parameter(2.0);
        col += color(r, world, 0);
      }
      // Average the color to anti-alias
      col /= float(NUM_SAMPLES);

      // Halve the color (gamma 2) by sqrt'ing 
      col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

      // Print out the pixel color
      int ir = int(255.99 * col[0]);
      int ig = int(255.99 * col[1]);
      int ib = int(255.99 * col[2]);
      std::cout << ir << " " << ig << " " << ib << "\n";
    }
  }
}

void generate_ppm_parallel(Hitable* world, const Camera& cam) {
  using namespace concurrency;
  using namespace std;

  // Allocate all the pixels on the heap w/ Vec of Vec3
  int pixel_count = RES_X * RES_Y;
  std::vector<std::array<int, 3>>* pixels = new vector<std::array<int, 3>>;
  pixels->reserve(pixel_count);
  for(int n = 0; n < pixel_count; ++n) {
    pixels->push_back({ 0, 0, 0 });
  }

  int num_samples = NUM_SAMPLES;

  parallel_for(0, pixel_count, [&](int n)
  {
      int row = RES_Y - (n / RES_X);
      int col = n % RES_X;

      Vec3 pixel;
      parallel_for(0, num_samples, [&](int p)
      {
        float u = (col + FRAND()) / RES_X;
        float v = (row + FRAND()) / RES_Y;
        Ray r = cam.get_ray(u, v);
        pixel += color(r, world, 0);
      });
      pixel /= NUM_SAMPLES;

      pixel = Vec3(sqrt(pixel.r()), sqrt(pixel.g()), sqrt(pixel.b()));

      pixels->at(n) = {
        int(255.99 * pixel.r()),
        int(255.99 * pixel.g()),
        int(255.99 * pixel.b())
      };
  });

  for(int p = 0; p < pixel_count; ++p) {
    std::cout << pixels->at(p)[0] << " " << pixels->at(p)[1] << " " << pixels->at(p)[2] << "\n";
  }
}

int main()
{
  using namespace concurrency;
  using namespace std;
  // PPM header
  cout << "P3\n" << RES_X << " " << RES_Y << "\n 255\n";
  vector<Hitable*> list;
  // Big sphere everything sits on
  list.push_back(new Sphere(Vec3(0, -1000, 0),
                            1000,
                            new Lambertian(Vec3(0.5, 0.5, 0.5))));
  
  parallel_for(-11, 11, [&](int a)
  {
      parallel_for(-11, 11, [&](int b)
      {
          float choose_mat = FRAND();
          Vec3 center(a + 0.9 * FRAND(), 0.2, b + 0.9 * FRAND());
          if((center - Vec3(4, 0.22, 0)).length() > 0.9) {
            if(choose_mat < 0.8) {
              list.push_back(new Sphere(center,
                                        0.2,
                                        new Lambertian(Vec3(FRAND()*FRAND(), FRAND()*FRAND(), FRAND()*FRAND()))));
            }
            else if(choose_mat < 0.95) {
              list.push_back(new Sphere(center,
                                        0.2,
                                        new Metal(Vec3(0.5*(1 + FRAND()), 0.5 * (1 + FRAND()), 0.5 * (1 + FRAND())), 0.5 * FRAND())));
            }
            else {
              list.push_back(new Sphere(center,
                                        0.2,
                                        new Dielectric(1.5)));
            }
          }
      });
  });
  
  list.push_back(new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5)));
  list.push_back(new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1))));
  list.push_back(new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0)));

  Hitable* world = new HitableList(list);
  
  Vec3 origin(13, 2, 3);
  Vec3 look_at(0, 0, 0.0);
  float fov = 20.0;
  float aspect = float(RES_X) / float(RES_Y);
  Camera cam(origin, look_at, Vec3(0, 1, 0), fov, aspect);
  
  //generate_ppm(world, cam);
  generate_ppm_parallel(world, cam);
  
  return 0;
}