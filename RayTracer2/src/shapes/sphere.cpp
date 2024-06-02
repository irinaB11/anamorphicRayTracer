#include "sphere.h"

#include <cmath>

#include "solvers.h"

using namespace std;

Hit Sphere::intersect(Ray const &ray) {
  // Sphere formula: ||x - position||^2 = r^2
  // Line formula:   x = ray.O + t * ray.D

  Vector L = ray.O - position;
  double a = ray.D.dot(ray.D);
  double b = 2.0 * ray.D.dot(L);
  double c = L.dot(L) - r * r;

  double t0;
  double t1;
  if (not Solvers::quadratic(a, b, c, t0, t1)) return Hit::NO_HIT();

  // t0 is closest hit
  if (t0 < 0.0)  // check if it is not behind the camera
  {
    t0 = t1;       // try t1
    if (t0 < 0.0)  // both behind the camera
      return Hit::NO_HIT();
  }

  // calculate normal
  Point hit = ray.at(t0);
  Vector N = (hit - position).normalized();

  // Note that the direction of the normal is not changed here,
  // but in scene.cpp - if necessary.

  return Hit(t0, N);
}

Vector Sphere::toUV(Point const &hit) {
  // Determine XYZ point to translate from
  Point point = hit - position;

  // Calculate local UV coordinates of the point
  double u = 0.5 + (std::atan2(point.y, point.x) / (2 * M_PI));
  double v = 1 - (std::acos(point.z / this->r) / M_PI);

  // Use a Vector to return 2 doubles. The third value is never read.
  return Vector(u, v, 0.0);
}

Sphere::Sphere(Point const &pos, double radius, Vector const &axis, double angle)
    :  // Feel free to modify this constructor.
      position(pos),
      r(radius),
      axis(axis),
      angle(angle) {}
