#include "Station.h"

Station::Station(int fuelPoints) {
	for (int i = 0; i < fuelPoints; i++) {
		FuelPoint* fuelPoint = new FuelPoint();
		this->fuelPoints.push_back(fuelPoint);
	}
}


Station::~Station() {
}
