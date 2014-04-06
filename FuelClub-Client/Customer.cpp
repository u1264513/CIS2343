#include "Customer.h"

#include "Vehicle.h"
#include "FuelPoint.h"

int Customer::id_count = 0;

/** Initialize customer object and set customer vehicle
 *  @param vehicle Vehicle customer owns
 */
Customer::Customer(Vehicle* vehicle) {
	this->vehicle = vehicle;
	this->loyalty = false;
	this->id = id_count;
	this->consume_ticks = 0;
	this->pay_ticks = 0;
	this->filled = false;
	id_count++;
}

/** Destroy customer vehicle object and reset id count
 */
Customer::~Customer() {
	this->id_count = 0;
	delete vehicle;
}

/** Returns true if customer has loyalty card
*/
bool Customer::isLoyalty() {
	return this->loyalty;
}

/** Sets loyalty card status of customer
 *  @param loyalty Status to set loyalty card
 */
bool Customer::setLoyalty(bool loyalty) {
	this->loyalty = loyalty;
	return this->loyalty;

}

/** Returns true if customer is in a queue
*/
bool Customer::inQueue() {
	return this->queue;
}

/** Sets queue status of customer
 *  @param queue Status to set qeue state
 */
bool Customer::setQueue(bool queue) {
	this->loyalty = queue;
	return this->queue;
}

/** Sets fuel point customer is at
 *  @param fuelPoint Pointer to fuel point customer is at
 */
FuelPoint* Customer::setFuelPoint(FuelPoint* fuelPoint) {
	this->fuelPoint = fuelPoint;
	return this->fuelPoint;
}

/** Returns fuel point object customer is at
 */
FuelPoint* Customer::getFuelPoint() {
	return this->fuelPoint;
}

/** Returns customers vehicle object
 */
Vehicle* Customer::getVehicle() {
	return this->vehicle;
}

/** Sets customers vehicle object
 *  @param vehicle Vehicle object to set
 */
Vehicle* Customer::setVehicle(Vehicle* vehicle) {
	this->vehicle = vehicle;
	return this->vehicle;
}

/** Returns ID of customer
 */
int Customer::getID() {
	return this->id;
}

/** Returns ticks taken to consume fuel
 */
int Customer::getConsumeTicks() {
	return this->consume_ticks;
}

/** Returns ticks taken to pay
 */
int Customer::getPayTicks() {
	return this->pay_ticks;
}

/** Set ticks taken to consume fuel
 *  @param consume Consume ticks to set
 */
int Customer::setConsumeTicks(int consume) {
	this->consume_ticks = consume;
	return this->consume_ticks;
}

/** Sets ticks taken to pay
 *  @param pay Pay ticks to set
 */
int Customer::setPayTicks(int pay) {
	this->pay_ticks = pay;
	return this->pay_ticks;
}

/** Returns true if customer has filled their vehicle
 */
bool Customer::hasFilled() {
	return this->filled;
}

/** Set customers filled status
 *  @param filled Status to set
 */
bool Customer::setFilled(bool filled) {
	this->filled = filled;
	return this->filled;
}