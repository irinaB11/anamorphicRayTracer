#include <cmath>
#include <iostream>

#include "cylinder.h"

#include "solvers.h"

using namespace std;

Hit Cylinder::intersect(Ray const &ray)
{   
    Vector H = (tPoint - bPoint).normalized();

    Vector X = ray.O - bPoint;

    double a = ray.D.dot(ray.D) - ray.D.dot(H) * ray.D.dot(H);
    double b = 2.0 * (ray.D.dot(X) - ray.D.dot(H) * X.dot(H));
    double c = X.dot(X) - X.dot(H) * X.dot(H) - r * r;

    double t0, t1;

    if (not Solvers::quadratic(a, b, c, t0, t1))
    {
        std::cout << "Couldn't solve quadratic\n";
        return Hit::NO_HIT();
    }

    //t0 is closest hit
    if (t0 < 0.0) // check if it is not behind the camera
    {
        t0 = t1;      // try t1
        if (t0 < 0.0) // both behind the camera
        {
            std::cout << "t0 < 0\n";
            return Hit::NO_HIT();
        }
    }


    Point hit = ray.at(t1);
    Vector N = (hit - position).normalized();

    return Hit(t1, N);
}

Point Cylinder::getPosition() {return position;}

Cylinder::Cylinder(Point const &pos, double radius, Point const& topPoint, Point const& bottomPoint)
    : // Feel free to modify this constructor.
      position(pos),
      r(radius),
      tPoint(topPoint),
      bPoint(bottomPoint)
{
}