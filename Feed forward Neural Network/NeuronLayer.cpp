#include "NeuronLayer.h"
#include <iostream>

void NeuronLayer::Init(unsigned int connectionsCount, unsigned int neuronCount)
{
	// in case of multiplay initializations remove old arrays
	delete[] weights;
	delete[] velocity;
	delete[] biases;
	delete[] z;
	delete[] activations;

	// save sizes
	this->neuronsCount = neuronCount;
	this->connectionsCount = connectionsCount;

	// create arrays
	weights		= new float[static_cast<long long>(connectionsCount) * neuronCount];
	velocity	= new float[static_cast<long long>(connectionsCount) * neuronCount];
	biases		= new float[neuronCount];
	z			= new float[neuronCount];
	activations = new float[neuronCount];
}

void NeuronLayer::PropagateError(const NeuronLayer& previous, const VectorLayer& errorIn, VectorLayer& errorOut) const
{

	assert(previous.neuronsCount == connectionsCount);
	assert(errorOut.neuronsCount >= previous.neuronsCount);
	assert(errorIn.neuronsCount >= neuronsCount);

	float* w = weights;
	float* z = previous.z;
	float* errOut = errorOut.activations;
	for (int j = 0; j < previous.neuronsCount; ++j)
	{
		float sum = 0;
		float* errIn = errorIn.activations;
		for (int k = 0; k < neuronsCount; ++k)
		{
			sum += *(w++) * *(errIn++);
		}
		*(errOut++) = sum *activationFunctionDerivate(*(z++));
	}
}

void NeuronLayer::GradientStep(float learnigSpeed, float regularization,  const VectorLayer& activations, const VectorLayer& error)
{
	assert(error.neuronsCount >= neuronsCount);

	float* w = weights;
	float* b = biases;
	float* err = error.activations;
	for (int j = 0; j < neuronsCount; ++j)
	{
		float* a = activations.activations;
		for (int k = 0; k < connectionsCount; ++k)
		{
			*(w++) -= learnigSpeed * *(a++) * *err + regularization * *w;
		}
		*(b++) -= learnigSpeed * *(err++);
	}
}

void NeuronLayer::GradientVelocityStep(float learnigSpeed, float damping, const VectorLayer& activations, const VectorLayer& error)
{
	float* w = weights;
	float* v = velocity;
	float* b = biases;
	float* err = error.activations;
	for (int j = 0; j < neuronsCount; ++j)
	{
		float* a = activations.activations;
		for (int k = 0; k < connectionsCount; ++k)
		{
			*v = *v * damping - learnigSpeed * *(a++) * *err;
			*(w++) += *(v++);
		}
		*(b++) -= learnigSpeed * *(err++);
	}
}

void NeuronLayer::ResetVelocity()
{
	float* v = velocity;
	for (int j = 0; j < neuronsCount; ++j)
	{
		for (int k = 0; k < connectionsCount; ++k)
		{
			*(v++) = 0;
		}
	}
}




void NeuronLayer::InitializeWeights(function<float()> fValue)
{
	float* w = weights;

	for (int j = 0; j < neuronsCount; ++j)
	{
		for (int k = 0; k < connectionsCount; ++k)
		{
			*(w++) = fValue();
		}
	}
}

void NeuronLayer::InitializeBiases(function<float()> fValue)
{
	float* b = biases;

	for (int j = 0; j < neuronsCount; ++j)
	{
		*(b++) = fValue();
	}
}

void NeuronLayer::DisplayWeightsBiases()
{
	float* w = weights;
	float* b = biases;

	cout << "Weights and biases:\n";
	for (int j = 0; j < neuronsCount; ++j)
	{
		cout << ">> neuron[" << j << "] :\n";
		cout << "	>> bias = " << *(b++) << "\n";

		for (int k = 0; k < connectionsCount; ++k)
		{
			cout << "	>> weight [" << k << "] = " << *(w++) << "\n";
		}
	}
}