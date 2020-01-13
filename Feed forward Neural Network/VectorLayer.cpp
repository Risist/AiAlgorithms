#include "VectorLayer.h"
#include "NeuralNetwork.h"
#include <iostream>

void VectorLayer::Init(unsigned int neuronsCount)
{
	delete[] activations;
	activations = new float[this->neuronsCount = neuronsCount];
}

void VectorLayer::Propagate(NeuronLayer& next) const
{
	assert(neuronsCount == next.connectionsCount);
	
	float* a = next.activations;
	float* b = next.biases;
	float* w = next.weights;
	float* z = next.z;

	for(int j = 0; j < next.neuronsCount; ++j)
	{
		float sum = 0;

		// compute activation input
		float* _x = activations;
		for (int k = 0; k < neuronsCount; ++k)
		{
			sum += *(w++) * *(_x++);
		}

		// update input sum and activation of the layer
		*z = sum + *(b++);
		*(a++) = next.activationFunction(*(z++));
	}
}

void VectorLayer::InitialError(const NeuronLayer& last, VectorLayer& error) const
{
	assert(last.neuronsCount == neuronsCount);
	assert(error.neuronsCount >= neuronsCount);

	float* e = error.activations;
	float* a = last.activations;
	float* y = activations;

	// cost function => cross entropy
	unsigned int n = neuronsCount;
	for (int i = 0; i < n; ++i)
	{
		*(e++) = *(a++) - *(y++);
	}
}

void VectorLayer::DisplayActivation() const
{
	cout << "Activations:\n";
	float* a = activations;
	for (int i = 0; i < neuronsCount; ++i)
	{
		cout << ">> " <<*(a++) << "\n";
	}
}


void VectorLayer::SetActivation(const initializer_list<float>& s)
{
	assert(s.size() <= neuronsCount);
	
	float* a = activations;
	for (auto it : s)
	{
		*(a++) = it;
	}
}