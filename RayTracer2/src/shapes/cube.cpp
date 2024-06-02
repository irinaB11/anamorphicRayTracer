#include "cube.h"

#include <cmath>

#include "solvers.h"

using namespace std;

double Cube::distance(Ray const& ray) {
    // returns the distance between the eye and a vertex of the cube

    double d = sqrt(ray.D.dot(ray.D));

    return d;
}

Cube::Cube(Point const& pos)
    :  // Feel free to modify this constructor.
    position(pos) {}
