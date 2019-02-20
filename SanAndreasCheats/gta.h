#pragma once
#include "types.h"

dword * const playerMoney = (dword*)0x00B7CE50;

class Pedestrian {
private:
	static const short health_offset = 0x540;
	static const short armor_offset = 0x548;
	dword address;

public:
	float* health;
	float* armor;

	Pedestrian() {
		address = 0;
	}

	bool isValid() {
		return address != 0;
	}

	void updateAddress(dword add) {
		if (address != add) {
			address = add;
			if (!isValid())
				return;
			health = (float*)(address + health_offset);
			armor = (float*)(address + armor_offset);
		}
	}
};

class Vehicle {
private:
	static const short health_offset = 0x4C0;
	dword address;

public:
	float* health;
	Vehicle() {
		address = 0;
	}

	bool isValid() {
		return address != 0;
	}

	void updateAddress(dword add) {
		if (address != add) {
			address = add;
			if (!isValid())
				return;
			health = (float*)(address + health_offset);
		}
	}
};