#pragma once
#include "Fuel.h"

class Fuel;

class Vehicle {

	enum Type {Car, Van, Bike, HGV};

public:
	Vehicle(Type type, Fuel::Type fuel);
	~Vehicle();

protected:
	Type type;
	Fuel::Type fuel;
	double consumed;
};
