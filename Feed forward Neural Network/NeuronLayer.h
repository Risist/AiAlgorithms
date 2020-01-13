#pragma once
#include "VectorLayer.h"
#include <functional>

class NeuronLayer : public VectorLayer
{
public:
	NeuronLayer() = default;
	NeuronLayer(unsigned int connectionsCount, unsigned int neuronCount)
	{
		Init(connectionsCount, neuronCount);
	}
	~NeuronLayer()
	{
		delete[] weights;
		delete[] velocity;
		delete[] biases;
		delete[] z;
	}

	void Init(unsigned int connectionsCount, unsigned int neuronCount);

	// propagates error onto previous layer.
	void PropagateError(const NeuronLayer& previous, const VectorLayer& errorIn, VectorLayer& errorOut) const;

	// does step of gradient descent optimalization in respect to computed error
	void GradientStep(float learnigSpeed, float regularization,const VectorLayer& activations, const VectorLayer& error);

	// does step of gradient descent optimalization in respect to computed error
	// version with coverange optimalisation by introducting velocity
	void GradientVelocityStep(float learnigSpeed, float damping, const VectorLayer& activations, const VectorLayer& error);

	// initializes values of weigts to a given function value
	void InitializeWeights(function<float()> fValue = []() {return 0.0f; });

	// initializes values of biases to a given function value
	void InitializeBiases(function<float()> fValue = []() {return 0.0f; });

	// prints value of weights and biases onto standard output
	void DisplayWeightsBiases();

	// resets velocity for gradient descent velocity optimalization
	void ResetVelocity();

	// activation function used in layer
	ActivationFunction activationFunction = Sigmoid;

	// activation function used in layer
	ActivationFunction activationFunctionDerivate = SigmoidDerivate;

	// how many connections are comming from previous layer
	unsigned int connectionsCount = 0;
	
	// weights of the layer
	// 2 dim array, weights[neuronId][connectionId]; // but compute manually
	float* weights = nullptr;
	
	// velocity, for learning speed optimalization
	// 2 dim array, velocity[neuronId][connectionId]; // but compute manually
	float* velocity = nullptr;
	
	// biases of the layer 
	// biases[neuronId]
	float* biases = nullptr;
	
	// last activation input sum
	// z[neuronId]
	float* z = nullptr;
};