#pragma once
class Vehicle;
class FuelPoint;

class Customer {
public:
	Customer(Vehicle* vehicle);
	~Customer();

	bool isLoyalty();
	bool setLoyalty(bool loyalty);

	bool inQueue();
	bool setQueue(bool queue);

	FuelPoint* setFuelPoint(FuelPoint* fuelPoint);
	FuelPoint* getFuelPoint();

	Vehicle* getVehicle();
	Vehicle* setVehicle(Vehicle* vehicle);

	int getConsumeTicks();
	int getPayTicks();

	int setConsumeTicks(int consume);
	int setPayTicks(int pay);

	bool hasFilled();
	bool setFilled(bool filled);

	int getID();

protected:
	int id;

	int consume_ticks;
	int pay_ticks;

	Vehicle* vehicle;
	FuelPoint* fuelPoint;

	bool loyalty;
	bool queue;
	bool filled;

	static int id_count;
};
