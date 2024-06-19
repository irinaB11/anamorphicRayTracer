#ifndef MESH_H_
#define MESH_H_

#include "../object.h"

class Mesh : public Object {

public:
	Mesh(Point const& pos);

	Point getPosition() override;

	Hit intersect(Ray const &ray) override;

	Point const position;
};

#endif