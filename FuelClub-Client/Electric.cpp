#include "Electric.h"


Electric::Electric() {
	setType(Fuel::Type::Electric);
}


Electric::~Electric() {

}

Fuel::Type Electric::setType(Type type) {
	if (type == Fuel::Type::Electric) {
		this->type = type;
	} else {
		printf("Error : Invalid fuel type for 'Electric' object!\n");
	}
	return this->type;
}
