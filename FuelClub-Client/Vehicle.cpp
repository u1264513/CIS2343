#include "Vehicle.h"

/** Initializes vehicle object
 *  @param type Vehicle type (Car, Bike, Van, HGV)
 *  @param fuel Fuel type (Petrol, Diesel, Propane, Electric)
 */
Vehicle::Vehicle(Type type, Fuel::Type fuel) {
	this->type = type;
	this->fuel = fuel;
	this->consumed = 0;
}

Vehicle::~Vehicle() {

}

/** Returns vehicle fuel type
 */
Fuel::Type Vehicle::getFuelType() {
	return this->fuel;
}

/** Sets vehicle fuel type
 *  @param type Fuel type
 */
Fuel::Type Vehicle::setFuelType(Fuel::Type type) {
	this->fuel = type;
	return this->fuel;
}

/** Returns consumed fuel from fuel point
 */
double Vehicle::getConsumed() {
	return this->consumed;
}

/** Sets consumed fuel from fuel point
 *  @param consumed Amount consumed
 */
double Vehicle::setConsumed(double consumed) {
	this->consumed = consumed;
	return this->consumed;
}

/** Increased consumed fuel by given amount
 *  @param consumed Amount to increase
 */
double Vehicle::addConsumed(double consumed) {
	this->consumed += consumed;
	return this->consumed;
}

/** Returns vehicle type
 */
Vehicle::Type Vehicle::getType() {
	return this->type;
}

/** Returns vehicle type string
 */
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

/** Returns vehicle fuel type string
 */
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
