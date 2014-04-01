#include "Combustible.h"


Combustible::Combustible() {
	
}


Combustible::~Combustible() {

}

Fuel::Type Combustible::setType(Type type) {
	if (type != Fuel::Type::Electric) {
		this->type = type;
	} else {
		printf("Error : Invalid fuel type for 'Combustible' object!\n");
	}
	return this->type;
}
