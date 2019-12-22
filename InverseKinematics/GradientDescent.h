#pragma once
#include "Vector.h"
#include "Data.h"

const float SAMPLING_DISTANCE = 0.001f;

float DistanceFromTarget(Vector2 target)
{
	ComputeChainPositions();
	return (target - GetEndEffector()).GetLength();
}

float PartialGradient(Vector2 target, int i)
{
	float angle = angles[i];

	//float f_x = DistanceFromTarget(target);
	angles[i] = angle - SAMPLING_DISTANCE;
	float f_x_minus_h = DistanceFromTarget(target);

	angles[i] = angle + SAMPLING_DISTANCE;
	float f_x_plus_h = DistanceFromTarget(target);

	float gradient = (f_x_plus_h - f_x_minus_h) / SAMPLING_DISTANCE;
	angles[i] = angle;
	return gradient;
}

void InverseKinematicsStep_GradientDescent(Vector2 target, float learingRate)
{
	for (int i = 0; i < CHAIN_SIZE - 1; ++i)
	{
		float gradient = PartialGradient(target, i);
		angles[i] -= learingRate * gradient;
	}
}
