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

/** Simulation constructor to initialize simulation
 *  @param client Client to send data to
 *  @param station Number of stations to simulate
 */
Simulation::Simulation(Client* client, int stations) {
	GUI::addListItem(ID_LIST, "Initializing Simulation...");

	//Setup simulation time
	this->simulation_time = (unsigned long)time(NULL);

	//Initialize random seed
	srand((unsigned int)GetTickCount());

	//Set fuel prices, currently hard-coded
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

		//Set station prices
		station->setPrices(fuelPrices);

		//Create 'Fuel' objects
		Combustible* petrol = new Combustible(Fuel::Type::Petrol);
		Combustible* diesel = new Combustible(Fuel::Type::Diesel);
		Combustible* propane = new Combustible(Fuel::Type::Propane);
		Electric* electric = new Electric();

		//Add fuel points to station
		int fuelPoints = fuelPoints_array[GenerateRandomNumber(0,2)];
		for (int j=0;j<fuelPoints;j++) {
			FuelPoint* fuelPoint = new FuelPoint();
			station->addFuelPoint(fuelPoint);

			//Randomize fuel at each fuel point
			//60% Petrol+Diesel, 30% Propane, 10% Electric
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

/** Unload all station objects
 */
Simulation::~Simulation() {
	for(int i=0;i<this->stations.size();i++)
		delete stations[i];
}

/** Generate a random number between the two intervals
 *  @param min
 *  @param max
 */
int Simulation::GenerateRandomNumber(int min, int max) {
	return rand() % max + min;
}

/** Simulation tick, the simulation completes one cycle every tick (each tick represents a second)
 */
void Simulation::Tick() {

	int customers = 0;
	double revenue = 0;

	//Loop through stations
	for(int i=0;i<this->stations.size();i++) {

		//Generate customers
		//25% chance of a customer arriving every minute
		if (GenerateRandomNumber(0, 3600) >= (3600-45)) {

			//Create vehicle for customer, randomizing vehicle type and fuel
			Vehicle* vehicle = new Vehicle((Vehicle::Type)GenerateRandomNumber(0, sizeof(Vehicle::Type)), (Fuel::Type)GenerateRandomNumber(0, sizeof(Fuel::Type)));
			Customer* customer = new Customer(vehicle);

			//Has loyalty card?
			//30% chance of having a loyalty card
			int random = GenerateRandomNumber(0, 100);
			if (random > 70)
				customer->setLoyalty(true);

			//Add customer to station
			this->stations[i]->addCustomer(customer);
		}

		//Consume fuel from fuel point
		//Loop through station fuel points
		for (int j=0;j<stations[i]->getFuelPoints()->size();j++) {
			FuelPoint* fuelPoint = stations[i]->getFuelPoints()->at(j);

			//Customer at fuel point?
			if (fuelPoint->getCustomer() != NULL) {
				Customer* customer = fuelPoint->getCustomer();

				//Calculate consume type dependant on vehicle type
				if (!customer->getConsumeTicks()) {
					int ticks = this->simulation_time + GenerateRandomNumber(5, 15);

					//Van
					if (customer->getVehicle()->getType() == Vehicle::Type::Van) {
						ticks += GenerateRandomNumber(10, 20);

					//HGV
					} else if (customer->getVehicle()->getType() == Vehicle::Type::HGV) {
						ticks += GenerateRandomNumber(20, 40);
					}

					//Set consumption time
					customer->setConsumeTicks(ticks);

				//Has consumed set amount?
				} else if (!customer->hasFilled() && (customer->getConsumeTicks() <= this->simulation_time)) {
					GUI::addListItem(ID_LIST, "%s CUSTOMER #%d : Filled vehicle", Timestamp::getTimestamp(this->simulation_time), customer->getID());
					customer->setFilled(true);

				//Consuming fuel...
				} else {
					customer->getVehicle()->addConsumed(0.8f);
				}

				//Calculate time taken to pay dependant on fuel point
				if (!customer->getPayTicks()) {
					int ticks = customer->getConsumeTicks();

					//Self-Serve fuel point, 10-20 second pay time
					if (fuelPoint->isSelfServe()) {
						ticks += GenerateRandomNumber(10, 20);

					//Manned - 60-90 second pay time
					} else {
						ticks += GenerateRandomNumber(60, 90);
					}

					//Set pay time
					customer->setPayTicks(ticks);

				//Pay?
				} else if(customer->getPayTicks() <= this->simulation_time) {
					stations[i]->customerPay(customer);
				}
			}
		}

		//Update statistics
		customers += stations[i]->getTransactions();
		revenue += stations[i]->getRevenue();
	}

	GUI::setText(ID_CUSTOMERS, "%d", customers);
	GUI::setText(ID_REVENUE, "£%0.2f", revenue);

	//Increase simulation time
	this->simulation_time++;
}
