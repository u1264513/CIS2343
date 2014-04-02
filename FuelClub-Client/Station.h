#pragma once
#include "FuelPoint.h"

#include <vector>

class Station {
public:
	Station(int fuelPoints);
	~Station();

	void addFuelPoint(FuelPoint* fuelPoint);
	void removeFuelPoint(FuelPoint* fuelPoint);

	void addCustomer();
	void removeCustomer(Customer* customer);

	void addQueue(Customer* customer);
	void removeQueue(Customer* customer);

	void customerPay(Customer* customer);

	double getRevenue();
	long getTransactions();

private:
	std::vector<FuelPoint*> fuelPoints;
	std::vector<Customer*> queue;

	int transactions;
	double revenue;
};
