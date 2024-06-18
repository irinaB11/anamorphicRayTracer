#ifndef CYLINDER_H_
#define CYLINDER_H_

#include "../object.h"

class Cylinder : public Object {
	double const PI = 3.14159265358979323846;

public:
	Cylinder(Point const& pos, double radius, Point const& topPoint, Point const& bottomPoint);

	//Point getPosition() override;

	Hit intersect(Ray const& ray) override;

	Point const position;
	double const r;
	Point const tPoint;
	Point const bPoint;
};

#endif
