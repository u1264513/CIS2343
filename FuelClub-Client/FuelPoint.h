#pragma once
#include "Fuel.h"
#include "Customer.h"

#include <vector>
using namespace std;

class FuelPoint {
public:
	FuelPoint();
	~FuelPoint();

	void addFuel(Fuel* fuel);
	void removeFuel(Fuel* fuel);

	bool isSelfServe();
	bool setSelfServe(bool isSelfServe);

	Customer* getCustomer();
	Customer* setCustomer(Customer* customer);

	bool hasFuelType(Fuel::Type type);

protected:
	vector<Fuel*> fuels;

	bool selfServe;
	Customer* customer;
};
