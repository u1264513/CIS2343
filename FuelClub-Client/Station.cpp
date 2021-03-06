#include "Station.h"

#include "FuelPoint.h"
#include "Customer.h"
#include "Vehicle.h"

#include "../Network/Client.h"
#include "../Network/Packet.h"

#include "Timestamp.h"

#include "GUI.h"

int Station::id_count = 0; //Current station id count

/** Create station object
 *  @param client Client to send data to
 *  @param timestamp Current timestamp
 */
Station::Station(Client* client, time_t* timestamp) {
	this->timestamp = timestamp;
	this->transactions = 0;
	this->revenue = 0;
	this->id = id_count;
	this->id_count++;
	this->client = client;
}

/** Unload all fuel points and queue
 */
Station::~Station() {
	id_count = 0;
	for(int i=0;i<this->fuelPoints.size();i++)
		delete fuelPoints[i];
	for(int i=0;i<this->queue.size();i++)
		delete queue[i];
}

/** Set fuel prices array
 *  @param fuelPrices Fuel prices array pointer
 */
void Station::setPrices(double* fuelPrices) {
	this->fuelPrices = fuelPrices;
}

/** Add fuel point to station
 *  @param fuelPoint Pointer to fuel point to add
 */
void Station::addFuelPoint(FuelPoint* fuelPoint) {
	this->fuelPoints.push_back(fuelPoint);
}

/** Remove fuel point from station
 *  @param fuelPoint Pointer to fuel point to remove
 */
void Station::removeFuelPoint(FuelPoint* fuelPoint) {
	for(unsigned i=0;i<fuelPoints.size();i++)
		if(fuelPoints[i] == fuelPoint) {
			fuelPoints.erase(fuelPoints.begin()+i);
			break;
		}
}

/** Add customer to station, add to queue if no fuel point available
 *  @param customer Customer to add
 */
void Station::addCustomer(Customer* customer) {
	if (hasFuelType(customer)) {
		for (unsigned i=0;i<fuelPoints.size(); i++)
			if (fuelPoints[i]->getCustomer() == NULL && fuelPoints[i]->hasFuelType(customer->getVehicle()->getFuelType())) {
				fuelPoints[i]->setCustomer(customer);
				GUI::addListItem(ID_LIST, "%s CUSTOMER #%d : %s %s arrived at pump %d", Timestamp::getTimestamp(*this->timestamp), customer->getID(), customer->getVehicle()->getFuelTypeString(), customer->getVehicle()->getVehicleTypeString(), i);
				return;
			}
		addQueue(customer);
	} else {
		GUI::addListItem(ID_LIST, "%s CUSTOMER #%d : Departed, doesn't sell requested fuel", Timestamp::getTimestamp(*this->timestamp), customer->getID());
	}
}

/** Remove customer from station
 *  @param customer Customer to remove
 */
void Station::removeCustomer(Customer* customer) {
	for(unsigned i=0;i<fuelPoints.size();i++)
		if(fuelPoints[i]->getCustomer() == customer) {
			fuelPoints[i]->setCustomer(NULL);
			break;
		}
	removeQueue(customer);
	GUI::addListItem(ID_LIST, "%s CUSTOMER #%d : Departed", Timestamp::getTimestamp(*this->timestamp), customer->getID());

	if (queue.size() > 0) {
		customer = queue.front();
		removeQueue(customer);
		customer->setQueue(false);
		addCustomer(customer);
	}
}

/** Add customer to queue
 *  @param customer Customer to add
 */
void Station::addQueue(Customer* customer) {
	customer->setQueue(true);
	this->queue.push_back(customer);
	GUI::addListItem(ID_LIST, "%s CUSTOMER #%d : Waiting in queue...", Timestamp::getTimestamp(*this->timestamp), customer->getID());
}

/** Remove customer from queue
 *  @param customer Customer to remove
 */
void Station::removeQueue(Customer* customer) {
	for(unsigned i=0;i<queue.size();i++)
		if(queue[i] == customer) {
			queue.erase(queue.begin()+i);
			break;
		}
}

/** Customer paying for fuel
 *  @param customer Customer thats paying
 */
void Station::customerPay(Customer* customer) {
	double paid = this->fuelPrices[customer->getVehicle()->getFuelType()]*customer->getVehicle()->getConsumed();

	//Has loyalty card?
	if (customer->isLoyalty()) {
		GUI::addListItem(ID_LIST, "%s CUSTOMER #%d : Has loyalty card, discount deducted", Timestamp::getTimestamp(*this->timestamp), customer->getID());
		paid *= 0.90;
	}

	//Update statistics
	this->revenue += paid;
	this->transactions++;

	GUI::addListItem(ID_LIST, "%s CUSTOMER #%d : Paid �%0.2f", Timestamp::getTimestamp(*this->timestamp), customer->getID(), paid);

	//Remove customer
	removeCustomer(customer);

	//Send to server
	char data[256];
	sprintf(data, "CUSTOMER #%d - %s %s - �%0.2f", customer->getID(), customer->getVehicle()->getFuelTypeString(), customer->getVehicle()->getVehicleTypeString(), paid);
	Packet* packet = new Packet((unsigned char*)data, strlen(data));
	client->Send(packet);
}

/** Return revenue
 */
double Station::getRevenue() {
	return this->revenue;
}

/** Return number of transactions
 */
long Station::getTransactions() {
	return this->transactions;
}

/** Add customer to queue
 *  @param customer Customer to add
 */
bool Station::hasFuelType(Customer* customer) {
	for (unsigned i=0;i<fuelPoints.size();i++)
		if (fuelPoints[i]->hasFuelType(customer->getVehicle()->getFuelType()))
			return true;
	return false;
}

/** Return pointer to fuel points
 */
std::vector<FuelPoint*>* Station::getFuelPoints() {
	return &fuelPoints;
}

/** Return station ID
 */
int Station::getID() {
	return this->id;
}