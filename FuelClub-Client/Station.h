#pragma once
class FuelPoint;
class Customer;

class Client;
class Packet;

#include <vector>

class Station {
public:
	Station(Client* client, time_t* timestamp);
	~Station();

	void setPrices(double* fuelPrices);

	void addFuelPoint(FuelPoint* fuelPoint);
	void removeFuelPoint(FuelPoint* fuelPoint);

	void addCustomer(Customer* customer);
	void removeCustomer(Customer* customer);

	void addQueue(Customer* customer);
	void removeQueue(Customer* customer);

	void customerPay(Customer* customer);

	double getRevenue();
	long getTransactions();

	bool hasFuelType(Customer* customer);

	std::vector<FuelPoint*>* getFuelPoints();

	int getID();

private:
	int id;

	std::vector<FuelPoint*> fuelPoints;
	std::vector<Customer*> queue;

	double* fuelPrices;

	int fuelPointsAvailable;

	int transactions;
	double revenue;

	static int id_count;

	time_t* timestamp;

	Client* client;
};
