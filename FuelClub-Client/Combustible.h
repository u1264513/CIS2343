#pragma once
#include "Fuel.h"

class Combustible : public Fuel {

public:
	Combustible(Type type);
	~Combustible();

	Type setType(Type type);
};
