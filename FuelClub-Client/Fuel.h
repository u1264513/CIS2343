#pragma once
#include "stdio.h"

class Fuel {

public:

	enum Type { Petrol, Diesel, Propane, Electric };

	Fuel();
	~Fuel();

	Type getType();

	virtual Type setType(Type type);

protected:
	Type type;
};
