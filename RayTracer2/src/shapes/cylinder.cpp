#include <cmath>

#include "cylinder.h"

#include "solvers.h"

using namespace std;

Hit Cylinder::intersect(Ray const& ray) {
    // The eye is positioned at the centre of the cylinder.
    // There is no rotation. The code should be adapted to take into 
    // consideration cases when the eye and the centre of the mirror
    // do not have the same position. 

    Point T, B; // The coordinates of the points at the centre of the 
                // cicles bounding the top and bottom of the cylinder.
    T.x = position.x;
    T.y = position.y + h / 2;
    T.x = position.z;

    B.x = position.x;
    B.y = position.y - h / 2;
    B.x = position.z;

    Vector H = B - T; // Direction of the vector starting at the top of 
                      //the cylinder going to toward the bottom
    Vector X = ray.O - T; // Direction of the vector going from the top
                          // of the cylinder to the eye position

    double a = ray.D.dot(D) - ray.D.dot(H) * ray.D.dot(H);
    double b = 2.0 * (ray.D.dot(X) - ray.D.dot(H) * X.dot(H));
    double c = X.dot(X) - X.dot(H) * X.dot(H) - r * r;

    double t0, t1;

    if (not Solvers::quadratic(a, b, c, t0, t1)) return Hit::NO_HIT();

    // t0 is closest hit
    if (t0 < 0.0)  // check if it is not behind the camera
    {
        t0 = t1;       // try t1
        if (t0 < 0.0)  // both behind the camera
            return Hit::NO_HIT();
    }

    Point hit = ray.at(t0);
    Vector N = (hit - position).normalized();

    return Hit(t0, N);
}

Point Cylinder::getPosition() {return position;}

Cylinder::Cylinder(Point const& pos, double radius, double const& height)
    :  // Feel free to modify this constructor.
    position(pos),
    r(radius),
    h(height) {}