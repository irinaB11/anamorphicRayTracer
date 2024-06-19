#include "mesh.h"

#include <cmath>

#include "solvers.h"

using namespace std;

Hit Mesh::intersect(Ray const &ray) {

  return Hit::NO_HIT();
}

Point Mesh::getPosition() {return position;}

Mesh::Mesh(Point const& pos)
    :  // Feel free to modify this constructor.
    position(pos) {}
