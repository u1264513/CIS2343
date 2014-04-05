#include "Fuel.h"

Fuel::Fuel() {

}

Fuel::~Fuel() {

}

double Fuel::getPrice() {
	return this->price;
}

Fuel::Type Fuel::getType() {
	return this->type;
}

double Fuel::getConsumption() {
	return this->consumption;
}

double Fuel::getAmount() {
	return this->amount;
}

double Fuel::setPrice(double price) {
	this->price = price;
	return this->price;
}

Fuel::Type Fuel::setType(Type type) {
	this->type = type;
	return this->type;
}

double Fuel::setConsumption(double consumption) {
	this->consumption = consumption;
	return this->consumption;
}

double Fuel::setAmount(double amount) {
	this->amount = amount;
	return this->amount;
}