#pragma once
#include "Fuel.h"

class Combustible : public Fuel {

public:
	Combustible();
	~Combustible();

	Type setType(Type type);
};
