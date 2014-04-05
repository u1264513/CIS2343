#include "FuelPoint.h"


FuelPoint::FuelPoint() {
	this->customer = NULL;
	this->selfServe = false;
}


FuelPoint::~FuelPoint() {
	for(int i=0;i<fuels.size();i++)
		delete fuels[i];
	delete customer;
}

void FuelPoint::addFuel(Fuel* fuel) {
	this->fuels.push_back(fuel);
}

void FuelPoint::removeFuel(Fuel* fuel) {
	for (unsigned i = 0; i<this->fuels.size(); i++)
		if (this->fuels[i] == fuel)
			this->fuels.erase(this->fuels.begin() + i);
}

bool FuelPoint::isSelfServe() {
	return this->selfServe;
}

bool FuelPoint::setSelfServe(bool selfServe) {
	this->selfServe = selfServe;
	return this->selfServe;
}

Customer* FuelPoint::getCustomer() {
	return this->customer;
}

Customer* FuelPoint::setCustomer(Customer* customer) {
	this->customer = customer;
	return this->customer;
}

bool FuelPoint::hasFuelType(Fuel::Type type) {
	for (int i=0;i<fuels.size();i++)
		if (fuels[i]->getType() == type)
			return true;
	return false;
}