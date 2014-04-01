#pragma once
#include "Fuel.h"

class Electric : public Fuel {

public:
	Electric();
	~Electric();

	Type setType(Type type);
};
