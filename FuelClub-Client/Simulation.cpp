#include "Simulation.h"

#include <time.h>

#include "Station.h"
#include "Fuel.h"
#include "Combustible.h"
#include "Electric.h"
#include "FuelPoint.h"
#include "Vehicle.h"

#include "Timestamp.h"

#include "GUI.h"

#include "../Network/Client.h"

Simulation::Simulation(Client* client, int stations) {
	GUI::addListItem(ID_LIST, "Initializing Simulation...");

	this->simulation_time = (unsigned long)time(NULL);

	//Initialize random seed
	srand((unsigned int)GetTickCount());

	double* fuelPrices = new double[sizeof(Fuel::Type)];
	fuelPrices[Fuel::Type::Petrol] = 1.38f;
	fuelPrices[Fuel::Type::Diesel] = 1.46f;
	fuelPrices[Fuel::Type::Propane] = 0.97f;
	fuelPrices[Fuel::Type::Electric] = 0.32f;

	//Create stations
	for (int i=0;i<stations;i++) {

		//Randomly generate fuel points
		int fuelPoints_array[] = { 4, 6, 8 };

		//Create 'Station' object
		Station* station = new Station(client, &this->simulation_time);
		this->stations.push_back(station);

		station->setPrices(fuelPrices);

		//Create 'Fuel' objects
		Combustible* petrol = new Combustible(Fuel::Type::Petrol);
		petrol->setPrice(fuelPrices[Fuel::Type::Petrol]);

		Combustible* diesel = new Combustible(Fuel::Type::Diesel);
		diesel->setPrice(fuelPrices[Fuel::Type::Diesel]);

		Combustible* propane = new Combustible(Fuel::Type::Propane);
		propane->setPrice(fuelPrices[Fuel::Type::Propane]);

		Electric* electric = new Electric();
		electric->setPrice(fuelPrices[Fuel::Type::Electric]);

		//Add fuel points
		int fuelPoints = fuelPoints_array[GenerateRandomNumber(0,2)];
		for (int j=0;j<fuelPoints;j++) {
			FuelPoint* fuelPoint = new FuelPoint();
			station->addFuelPoint(fuelPoint);

			/*
			 * 60% Petrol+Diesel, 30% Propane, 10% Electric
			 */
			int random = GenerateRandomNumber(0,100);
			if (random >= 90) { //Electric
				fuelPoint->addFuel(electric);

			} else if (random >= 60) { //Propane
				fuelPoint->addFuel(propane);

			} else if (random >= 0) { //Petrol+Diesel
				fuelPoint->addFuel(petrol);
				fuelPoint->addFuel(diesel);

			}
		}

	}


}

Simulation::~Simulation() {
	for(int i=0;i<this->stations.size();i++)
		delete stations[i];
}

int Simulation::GenerateRandomNumber(int min, int max) {
	return rand() % max + min;
}

void Simulation::Tick() {

	int customers = 0;
	double revenue = 0;

	for(int i=0;i<this->stations.size();i++) {

		//Customer Arrival
		if (GenerateRandomNumber(0, 3600) >= (3600-45)) { //25% chance of arrival every minute
			Vehicle* vehicle = new Vehicle((Vehicle::Type)GenerateRandomNumber(0, sizeof(Vehicle::Type)), (Fuel::Type)GenerateRandomNumber(0, sizeof(Fuel::Type)));
			Customer* customer = new Customer(vehicle);

			//30% chance of loyalty card
			int random = GenerateRandomNumber(0, 100);
			if (random > 70)
				customer->setLoyalty(true);

			this->stations[i]->addCustomer(customer);
		}

		//Consume
		for (int j=0;j<stations[i]->getFuelPoints()->size();j++) {
			FuelPoint* fuelPoint = stations[i]->getFuelPoints()->at(j);

			if (fuelPoint->getCustomer() != NULL) {
				Customer* customer = fuelPoint->getCustomer();

				//Consume Time
				if (!customer->getConsumeTicks()) {
					int ticks = this->simulation_time + GenerateRandomNumber(5, 15);

					//Van
					if (customer->getVehicle()->getType() == Vehicle::Type::Van) {
						ticks += GenerateRandomNumber(10, 20);

					//HGV
					} else if (customer->getVehicle()->getType() == Vehicle::Type::HGV) {
						ticks += GenerateRandomNumber(20, 40);
					}

					customer->setConsumeTicks(ticks);

				} else if (!customer->hasFilled() && (customer->getConsumeTicks() <= this->simulation_time)) {
					GUI::addListItem(ID_LIST, "%s CUSTOMER #%d : Filled vehicle", Timestamp::getTimestamp(this->simulation_time), customer->getID());
					customer->setFilled(true);
				} else {
					customer->getVehicle()->addConsumed(0.8f);
				}

				if (!customer->getPayTicks()) {
					int ticks = customer->getConsumeTicks();

					if (fuelPoint->isSelfServe()) {
						ticks += GenerateRandomNumber(10, 20);
					} else {
						ticks += GenerateRandomNumber(60, 90);
					}

					customer->setPayTicks(ticks);

				} else if(customer->getPayTicks() <= this->simulation_time) {
					stations[i]->customerPay(customer);
				}
			}
		}

		customers += stations[i]->getTransactions();
		revenue += stations[i]->getRevenue();
	}

	GUI::setText(ID_CUSTOMERS, "%d", customers);
	GUI::setText(ID_REVENUE, "£%0.2f", revenue);

	this->simulation_time++;
}
