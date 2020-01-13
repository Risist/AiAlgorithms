#include "NeuralNetwork.h"
#include <iostream>

void NeuralNetwork::Init(const initializer_list<int>& neuronsInLayers)
{
	inputs			.Init( * neuronsInLayers.begin()   );
	expectedOutput	.Init( *(neuronsInLayers.end() - 1));
	
	
	layerCount = neuronsInLayers.size() - 1;
	delete[] layers;
	layers = new NeuronLayer[layerCount];

	unsigned int maxNeuronsInLayer = *neuronsInLayers.begin();
	auto itL = layers;

	auto end = neuronsInLayers.end() - 1;
	for (auto it = neuronsInLayers.begin(); it != end; ++it)
	{
		int m1 = *it;
		int m2 = *(it + 1);
		itL->Init(m1, m2);
		++itL;
		maxNeuronsInLayer = fmax(maxNeuronsInLayer, *(it + 1));
	}

	error[0].Init(maxNeuronsInLayer);
	error[1].Init(maxNeuronsInLayer);
}

const VectorLayer& NeuralNetwork::Propagate()
{
	assert(layerCount >= 1);

	NeuronLayer* layer = layers;
	inputs.Propagate(*layer);
	for (int l = 0; l < layerCount-1; ++l)
	{
		layer->Propagate( *(layer+1) );
		++layer;
	}

	return *layer;
}


void NeuralNetwork::BackPropagation(float learningSpeed, float regularization)
{
	int errorId = 0;

	NeuronLayer* layer = layers + (layerCount - 1);
	expectedOutput.InitialError(*layer, error[errorId]);
	for (; layer != layers; --layer)
	{
		layer->PropagateError(*(layer -1), error[errorId], error[(errorId + 1) % 2]);
		layer->GradientStep(learningSpeed, regularization, *(layer - 1), error[errorId]);
		errorId = (errorId + 1) % 2;
	}

	if (layerCount > 0)
	{
		layers->GradientStep(learningSpeed, regularization, inputs, error[errorId]);
	}
}

void NeuralNetwork::BackPropagationVelocity(float learningSpeed, float damping)
{
	int errorId = 0;

	NeuronLayer* layer = layers + (layerCount - 1);
	expectedOutput.InitialError(*layer, error[errorId]);
	ResetVelocity();
	for (; layer != layers; --layer)
	{
		layer->PropagateError(*(layer - 1), error[errorId], error[(errorId + 1) % 2]);
		layer->GradientVelocityStep(learningSpeed, damping,*(layer - 1), error[errorId]);
		errorId = (errorId + 1) % 2;
	}

	if (layerCount > 0)
	{
		layers->GradientStep(learningSpeed, damping, inputs, error[errorId]);
	}
}

void NeuralNetwork::InitializeWeights(function<float()> fValue)
{
	NeuronLayer* layer = layers;
	for (int i = 0; i < layerCount; ++i)
	{
		(layer++)->InitializeWeights(fValue);
	}
}

void NeuralNetwork::InitializeBiases(function<float()> fValue)
{
	NeuronLayer* layer = layers;
	for (int i = 0; i < layerCount; ++i)
	{
		(layer++)->InitializeBiases(fValue);
	}
}

void NeuralNetwork::DisplayWeightsBiases() const
{
	NeuronLayer* layer = layers;
	for (int i = 0; i < layerCount; ++i)
	{
		cout << "layer[" << i << "]-> ";
		(layer++)->DisplayWeightsBiases();
	}
}

void NeuralNetwork::DisplayLayersConnections() const
{
	NeuronLayer* layer = layers;
	for (int i = 0; i < layerCount; ++i)
	{
		cout << "layer[" << i << "] : ";
		cout << ">> connectionsCount = " << layer->connectionsCount << "\n";
		cout << ">> neuronsCount = "	 << layer->neuronsCount		<< "\n";
		++layer;
	}
}

void NeuralNetwork::ResetVelocity()
{
	NeuronLayer* layer = layers;
	for (int i = 0; i < layerCount; ++i)
	{
		(layer++)->ResetVelocity();
	}
}