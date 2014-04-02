#pragma once

class Station;

#include "../Network/Client.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

class Simulation {

public:
	Simulation(Client* client, int stations);
	~Simulation();

private:
	std::vector<Station*> stations;

	int GenerateRandomNumber(int min, int max);
};
