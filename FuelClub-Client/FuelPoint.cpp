#include "FuelPoint.h"


FuelPoint::FuelPoint() {

}


FuelPoint::~FuelPoint() {

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
