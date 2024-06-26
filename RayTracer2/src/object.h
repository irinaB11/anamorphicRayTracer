#ifndef OBJECT_H_
#define OBJECT_H_

// not really needed here, but deriving classes may need them
#include <memory>

#include "hit.h"
#include "ray.h"
#include "triple.h"
class Object;
typedef std::shared_ptr<Object> ObjectPtr;

class Object
{
public:
  virtual Point getPosition() = 0; //implement in every derived class

  virtual ~Object() = default;

  virtual Hit intersect(Ray const &ray) = 0; // must be implemented in derived class

  virtual Vector toUV(Point const &hit)
  {
    // bogus implementation
    return Vector{};
  }
};

#endif
