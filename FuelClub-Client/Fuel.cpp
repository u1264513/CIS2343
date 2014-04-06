#include "Fuel.h"

Fuel::Fuel() {

}

Fuel::~Fuel() {

}

/** Returns fuel type
 */
Fuel::Type Fuel::getType() {
	return this->type;
}

/** Sets vehicle fuel type
 *  @param type Fuel type
 */
Fuel::Type Fuel::setType(Type type) {
	this->type = type;
	return this->type;
}
