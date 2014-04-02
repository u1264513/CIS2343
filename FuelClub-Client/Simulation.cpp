#include "Simulation.h"
#include "Station.h"

Simulation::Simulation(Client* client, int stations) {
	printf("Initializing Simulation...\n");

	//Initialize random seed
	srand((unsigned int)time(NULL));

	//Create stations
	for (int i = 0; i < stations; i++) {

		//Randomly generate fuel points
		int fuelPoints[] = { 4, 6, 8 };

		//Create 'Station' object
		Station* station = new Station(fuelPoints[GenerateRandomNumber(0,2)]);
		this->stations.push_back(station);
	}


}

Simulation::~Simulation() {

}

int Simulation::GenerateRandomNumber(int min, int max) {
	return rand() % max + min;
}
