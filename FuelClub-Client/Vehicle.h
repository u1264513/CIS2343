#pragma once
#include "Fuel.h"

class Fuel;

class Vehicle {

public:
	enum Type {Car, Van, Bike, HGV};

	Vehicle(Type type, Fuel::Type fuel);
	~Vehicle();

	Fuel::Type getFuelType();
	Fuel::Type setFuelType(Fuel::Type type);

	double getConsumed();
	double setConsumed(double consumed);
	double addConsumed(double consumed);

	Vehicle::Type getType();
	char* getVehicleTypeString();
	char* getFuelTypeString();

protected:
	Type type;
	Fuel::Type fuel;
	double consumed;
};
