#include "Vehicle.h"

Vehicle::Vehicle(Type type, Fuel::Type fuel) {
	this->type = type;
	this->fuel = fuel;
	this->consumed = 0;
}

Vehicle::~Vehicle() {

}

Fuel::Type Vehicle::getFuelType() {
	return this->fuel;
}

Fuel::Type Vehicle::setFuelType(Fuel::Type type) {
	this->fuel = type;
	return this->fuel;
}

double Vehicle::getConsumed() {
	return this->consumed;
}

double Vehicle::setConsumed(double consumed) {
	this->consumed = consumed;
	return this->consumed;
}

double Vehicle::addConsumed(double consumed) {
	this->consumed += consumed;
	return this->consumed;
}

Vehicle::Type Vehicle::getType() {
	return this->type;
}

char* Vehicle::getVehicleTypeString() {
	if (this->type == Type::Car) {
		return "Car";
	} else if (this->type == Type::Van) {
		return "Van";
	} else if (this->type == Type::Bike) {
		return "Bike";
	} else if (this->type == Type::HGV) {
		return "HGV";
	}
	return NULL;
}

char* Vehicle::getFuelTypeString() {
	if (this->fuel == Fuel::Type::Petrol) {
		return "Petrol";
	} else if (this->fuel == Fuel::Type::Diesel) {
		return "Diesel";
	} else if (this->fuel == Fuel::Type::Propane) {
		return "Propane";
	} else if (this->fuel == Fuel::Type::Electric) {
		return "Electric";
	}
	return NULL;
}