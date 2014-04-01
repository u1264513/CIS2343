#include "Fuel.h"

Fuel::Fuel() {

}

Fuel::~Fuel() {

}

Fuel::Type Fuel::getType() {
	return type;
}

double Fuel::getPrice() {
	return price;
}

double Fuel::getConsumption() {
	return consumption;
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
