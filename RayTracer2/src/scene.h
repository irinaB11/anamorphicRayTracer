#ifndef SCENE_H_
#define SCENE_H_

#include <utility>
#include <vector>

#include "objloader.h"
#include "object.h"
#include "triple.h"

// Forward declarations
class Ray;
class Image;

class Scene
{
  std::vector<ObjectPtr> objects;

  struct intVector {
    int x;
    int y;
    int z;
  };

  Point eye;

  // Offset multiplier. Before casting a new ray from a hit point,
  // move the hit point in the direction of the normal with this offset
  // to prevent finding an intersection with the same object due to
  // floating point inaccuracies. This prevents shadow acne, among other problems.
  double const epsilon = 1E-3;
  double const PI = 3.14159265358979323846;

public:
  Scene();

  // determine closest hit (if any)
  std::pair<ObjectPtr, Hit> castRay(Ray const &ray) const;

  // trace a ray into the scene and return the color
  Point trace(Ray const &ray);

  // render the scene to the given image
  void render(std::string const &filePath, std::string const &ofname);

  void addObject(ObjectPtr obj);

  void setEye(Triple const &position);

  unsigned getNumObject();

  Point getEye();

private:
  
};

#endif
