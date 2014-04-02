#pragma once
#include "stdio.h"

class Fuel {

public:

	enum Type { Petrol, Diesel, Propane, Electric };

	Fuel();
	~Fuel();

	Type getType();
	double getPrice();
	double getConsumption();
	double getAmount();

	virtual Type setType(Type type);
	double setPrice(double price);
	double setConsumption(double consumption);
	double setAmount(double amount);

protected:
	Type type;
	double price;
	double consumption;
	double amount;

};
