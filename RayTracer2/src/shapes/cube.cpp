#include "cube.h"

#include <cmath>

#include "solvers.h"

using namespace std;

Hit Cube::intersect(Ray const &ray) {

  return Hit::NO_HIT();
}

//Point getPosition() {return position;}

Cube::Cube(Point const& pos)
    :  // Feel free to modify this constructor.
    position(pos) {}
