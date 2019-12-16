#include "Data.h"

float lengths[CHAIN_SIZE - 1] = {
	1, 1,
};

float angles[CHAIN_SIZE - 1] = {
	ToRadian(45),
	ToRadian(-45)
};
Vector2 positions[CHAIN_SIZE];