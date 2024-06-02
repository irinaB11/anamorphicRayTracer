#ifndef CUBE_H_
#define CUBE_H_

#include "../object.h"

class Cube : public Object {

public:
	Cube(Point const& pos);

	double Cube::distance(Ray const& ray);

	Point const position;
};

#endif