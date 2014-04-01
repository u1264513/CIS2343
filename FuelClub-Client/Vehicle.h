#pragma once
#include "Fuel.h"

class Fuel;

class Vehicle {

	enum Type {Car, Van, Bike, HGV};

public:
	Vehicle(Type type, Fuel::Type fuel);
	~Vehicle();

	bool inQueue();

	bool setQueueStatus(bool inQueue);

protected:
	Type type;
	Fuel* fuel;
	double consumed;

	bool queue;
};
