#include "Fuel.h"

Fuel::Fuel() {

}

Fuel::~Fuel() {

}

Fuel::Type Fuel::getType() {
	return this->type;
}

double Fuel::getPrice() {
	return this->price;
}

double Fuel::getConsumption() {
	return this->consumption;
}

double Fuel::getAmount() {
	return this->amount;
}

Fuel::Type Fuel::setType(Type type) {
	this->type = type;
	return this->type;
}

double Fuel::setPrice(double price) {
	this->price = price;
	return this->price;
}

double Fuel::setConsumption(double consumption) {
	this->consumption = consumption;
	return this->consumption;
}

double Fuel::setAmount(double amount) {
	this->amount = amount;
	return this->amount;
}