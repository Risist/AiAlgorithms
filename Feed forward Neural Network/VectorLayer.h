#pragma once
#include "Common.h"

class NeuronLayer;

class VectorLayer
{
public:
	VectorLayer() = default;
	VectorLayer(unsigned int neuronCount)
	{
		Init(neuronCount);
	}
	~VectorLayer()
	{
		delete[] activations;
	}

	void Init(unsigned int neuronsCount);
	
	// propagates forward impulse from the layer
	void Propagate(NeuronLayer& next) const;

	// fills error vector with initial errors, futher to be propagate throughout the network for gradient descent
	// this vector is an expected value vector which is compared with provided layer
	// cross entropy cost function is used
	void InitialError(const NeuronLayer& previous, VectorLayer& error) const;

	void SetActivation(const initializer_list<float>& s);

	// prints activation values on standard output
	void DisplayActivation() const;
	
	// how many neurons are placed in this layer
	unsigned int neuronsCount = 0;

	// activations of layer
	// activations[neuronId];
	float* activations = nullptr;

	friend class NeuronLayer;
};