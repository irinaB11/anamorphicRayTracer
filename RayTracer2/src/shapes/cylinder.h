#ifndef CYLINDER_H_
#define CYLINDER_H_

#include "../object.h"

class Cylinder : public Object {
	double const PI = 3.14159265358979323846;

public:
	Cylinder(Point const& pos, double radius, double const& height);

	Hit intersect(Ray const& ray) override;

	Point const position;
	double const r;
	double const h;
};

#endif
