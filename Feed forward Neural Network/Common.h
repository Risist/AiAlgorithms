#pragma once
#include <cstdint>
#include <assert.h>
#include <math.h>
#include <functional>

using namespace std;

using ActivationFunction = function<float(float)>;

// activation functions
inline float Sigmoid(float x)
{
	return 1 / (1 + exp(-x) );
}
inline float SigmoidDerivate(float x)
{
	float ex = exp(x);
	float k = ex + 1;
	return ex / (k * k);
}
