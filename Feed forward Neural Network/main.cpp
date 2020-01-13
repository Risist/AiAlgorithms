#include <iostream>
#include "NeuralNetwork.h"

#include <stdlib.h>
#include <ctime>

int main()
{
	srand(time(nullptr));
	NeuralNetwork network({ 1, 3, 2 });
	

	auto randomV = []() {return rand() % 1000 * 0.001f; };
	network.InitializeWeights( randomV);
	network.InitializeBiases ( randomV);

	//network.DisplayLayersConnections();
	//network.DisplayWeightsBiases();
	

	network.SetExpectedOutput({0.5, 0.75f});
	
	int i = 0;
	while (true)
	{
		cout << i++ << "\n";
		network.SetInput({ randomV() });
		network.Propagate().DisplayActivation();
		//network.BackPropagation(0.35f, 0.1f);
		network.BackPropagationVelocity(2.f, 0.95f);

		system("pause");
	}
	


}