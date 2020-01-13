#pragma once
#include "NeuronLayer.h"

class NeuralNetwork
{
public:
	NeuralNetwork() = default;
	NeuralNetwork(const initializer_list<int>& neuronsInLayers)
	{
		Init(neuronsInLayers);
	}

	void Init(const initializer_list<int>& neuronsInLayers);

	const VectorLayer& Propagate();

	void BackPropagation(float learningSpeed, float regularization = 0.0f);
	void BackPropagationVelocity(float learningSpeed, float damping = 0.0f);

	void SetInput(const initializer_list<float>& x)
	{
		inputs.SetActivation(x);
	}
	void SetExpectedOutput(const initializer_list<float>& x)
	{
		expectedOutput.SetActivation(x);
	}

	void InitializeWeights(function<float()> fValue);
	void InitializeBiases(function<float()> fValue);
	void ResetVelocity();

	// prints value of weights and biases from all layers onto standard output
	void DisplayWeightsBiases() const;

	void DisplayLayersConnections() const;
	
	unsigned int layerCount = 0;
	NeuronLayer* layers = nullptr;
	VectorLayer inputs;
	VectorLayer expectedOutput;
	VectorLayer error[2];
};