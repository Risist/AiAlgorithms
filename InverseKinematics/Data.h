#pragma once
#include "Vector.h"

static const unsigned int CHAIN_SIZE = 8;
extern float lengths[CHAIN_SIZE - 1];
extern float angles[CHAIN_SIZE - 1];
extern Vector2 positions[CHAIN_SIZE];

inline void ComputeChainPositions()
{
	positions[0] = { 0,0 };
	for (int i = 1; i < CHAIN_SIZE; ++i)
		positions[i] = positions[i - 1] + Vector2{ lengths[i - 1], 0 }.GetRotated(angles[i - 1]);
}
inline Vector2 GetEndEffector()
{
	return positions[CHAIN_SIZE - 1];
}

inline void DisplayChainPositions()
{
	cout << "Positions:\n";
	for (int i = 0; i < CHAIN_SIZE; ++i)
		cout << ">> " << positions[i].x << ", " << positions[i].y << ",\n";
}
inline void DisplayChainAngles()
{
	cout << "Angles:\n";
	for (int i = 0; i < CHAIN_SIZE - 1; ++i)
		cout << ">> " << ToDegree(angles[i]) << ",\n";
}