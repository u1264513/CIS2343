#include "FuelPoint.h"

/** Initializes FuelPoint object
 */
FuelPoint::FuelPoint() {
	this->customer = NULL;
	this->selfServe = false;
}

/** Unloads all fuel and customer objects from fuel point
 */
FuelPoint::~FuelPoint() {
	for(int i=0;i<fuels.size();i++)
		delete fuels[i];
	delete customer;
}

/** Adds fuel to fuel point
 *  @param fuel Pointer to fuel object to add
 */
void FuelPoint::addFuel(Fuel* fuel) {
	this->fuels.push_back(fuel);
}

/** Removes fuel from fuel point
 *  @param fuel Pointer to fuel object to remove
 */
void FuelPoint::removeFuel(Fuel* fuel) {
	for (unsigned i = 0; i<this->fuels.size(); i++)
		if (this->fuels[i] == fuel)
			this->fuels.erase(this->fuels.begin() + i);
}

/** Returns true if fuel point is self-serve
 */
bool FuelPoint::isSelfServe() {
	return this->selfServe;
}

/** Sets self-serve status of fuel point
 *  @param selfServe Boolean of to set self-serve status
 */
bool FuelPoint::setSelfServe(bool selfServe) {
	this->selfServe = selfServe;
	return this->selfServe;
}

/** Returns current customer at the fuel point
 */
Customer* FuelPoint::getCustomer() {
	return this->customer;
}

/** Sets current customer at the fuel point
 *  @param customer Customer to set at fuel point
 */
Customer* FuelPoint::setCustomer(Customer* customer) {
	this->customer = customer;
	return this->customer;
}

/** Returns true if fuel point has specified fuel type
 *  @param type Fuel type to check
 */
bool FuelPoint::hasFuelType(Fuel::Type type) {
	for (int i=0;i<fuels.size();i++)
		if (fuels[i]->getType() == type)
			return true;
	return false;
}