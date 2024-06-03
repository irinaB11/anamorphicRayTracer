#ifndef CUBE_H_
#define CUBE_H_

#include "../object.h"

class Cube : public Object {

public:
	Cube(Point const& pos);

	//Point getPosition() override;

	Hit intersect(Ray const &ray) override;

	Point const position;
};

#endif