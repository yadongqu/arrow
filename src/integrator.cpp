

#include "integrator.h"

#include "hittable.h"
#include "record.h"
#include "sampling.h"

glm::vec3 TestIntegrator::li(const Ray &ray, const Scene &scene,
                             int depth) const {
  if (depth < 0) {
    return glm::vec3(0.0f);
  }
  HitRecord rec;
  if (scene.hit(ray, rec)) {

    // return (rec.normal + glm::vec3(1.0f, 1.0f, 1.0f)) * 0.5f;
    const auto &object = scene.get(rec.object_id.value());
    glm::vec3 attenuation;
    Ray scattered;

    if (object->material()->scatter(ray, rec, attenuation, scattered)) {
      return attenuation * li(scattered, scene, depth - 1);
    }
    return attenuation;
  }

  glm::vec3 unit_direction = glm::normalize(ray.direction());
  float t = 0.5f * (unit_direction.y + 1.0f);
  return (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) +
         t * glm::vec3(0.5f, 0.7f, 1.0f);
}

glm::vec3 WhitIntegrator::li(const Ray &ray, const Scene &scene,
                             int depth) const {
  if (depth < 0) {
    return glm::vec3(0.0f);
  }
  HitRecord rec;
  if (!scene.hit(ray, rec)) {
    return glm::vec3(0.0f);
  }
  const auto &object = scene.get(rec.object_id.value());
  glm::vec3 attenuation;
  Ray scattered;

  glm::vec3 emitted = object->material()->emitted(ray, rec);

  if (object->material()->scatter(ray, rec, attenuation, scattered)) {
    return emitted + attenuation * li(scattered, scene, depth - 1);
  }

  return emitted;
}

glm::vec3 PathIntegrator::li(const Ray &ray, const Scene &scene,
                             int depth) const {}

glm::vec3 NormalIntegrator::li(const Ray &ray, const Scene &scene,
                               int depth) const {
  HitRecord rec;
  if (!scene.hit(ray, rec)) {
    return glm::vec3(0.0f);
  }
  return (rec.normal + glm::vec3(1.0f, 1.0f, 1.0f)) * 0.5f;
}

glm::vec3 VisibilityIntegrator::li(const Ray &ray, const Scene &scene,
                                   int depth) const {
  HitRecord rec;
  if (scene.hit(ray, rec)) {
    if (glm::dot(ray.direction(), rec.normal) < 0.0f) {
      return glm::vec3(1.0f);
    }
    return glm::vec3(0.0f);
  }
  return glm::vec3(0.0f);
}