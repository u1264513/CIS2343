#include "Customer.h"

#include "Vehicle.h"
#include "FuelPoint.h"

int Customer::id_count = 0;

Customer::Customer(Vehicle* vehicle) {
	this->vehicle = vehicle;
	this->loyalty = false;
	this->id = id_count;
	this->consume_ticks = 0;
	this->pay_ticks = 0;
	this->filled = false;
	id_count++;
}

Customer::~Customer() {
	this->id_count = 0;
	delete vehicle;
}

bool Customer::isLoyalty() {
	return this->loyalty;
}

bool Customer::setLoyalty(bool loyalty) {
	this->loyalty = loyalty;
	return this->loyalty;

}

bool Customer::inQueue() {
	return this->queue;
}

bool Customer::setQueue(bool queue) {
	this->loyalty = queue;
	return this->queue;
}

FuelPoint* Customer::setFuelPoint(FuelPoint* fuelPoint) {
	this->fuelPoint = fuelPoint;
	return this->fuelPoint;
}

FuelPoint* Customer::getFuelPoint() {
	return this->fuelPoint;
}

Vehicle* Customer::getVehicle() {
	return this->vehicle;
}

Vehicle* Customer::setVehicle(Vehicle* vehicle) {
	this->vehicle = vehicle;
	return this->vehicle;
}

int Customer::getID() {
	return this->id;
}

int Customer::getConsumeTicks() {
	return this->consume_ticks;
}

int Customer::getPayTicks() {
	return this->pay_ticks;
}

int Customer::setConsumeTicks(int consume) {
	this->consume_ticks = consume;
	return this->consume_ticks;
}

int Customer::setPayTicks(int pay) {
	this->pay_ticks = pay;
	return this->pay_ticks;
}

bool Customer::hasFilled() {
	return this->filled;
}
bool Customer::setFilled(bool filled) {
	this->filled = filled;
	return this->filled;
}