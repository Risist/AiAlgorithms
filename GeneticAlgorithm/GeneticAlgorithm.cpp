#include <iostream>
#include <cstdint>
#include <string>
#include <set>
#include <unordered_set>
#include <vector>
#include <assert.h>
#include <limits>
#include <queue>
#include <cmath>
#include <ctime>

using namespace std;

using Chromosome = uint32_t;

Chromosome Crossover(Chromosome c1, Chromosome c2)
{
	int point = rand() % (sizeof(Chromosome) * 8);
	Chromosome mask = -1;
	mask = mask << point;
	return c1 & mask | c2 & (~mask);
}
Chromosome Crossover(Chromosome c1, Chromosome c2, int point)
{
	Chromosome mask = -1;
	mask = mask << point;
	return c1 & mask | c2 & (~mask);
}

void DrawBits(Chromosome c)
{
	while (c)
	{
		cout << (c & 1);
		c = (c >> 1);
	}
	cout << "\n";
}
void DrawAllBits(Chromosome c)
{
	for (int i = 0; i < sizeof(c) * 8; ++i)
	{
		cout << (c & 1);
		c = (c >> 1);
	}
	cout << "\n";
}

vector<Chromosome> population;
Chromosome GetRandomChromosome()
{
	return rand();
}
void InitPopulation(int n)
{
	population.reserve(n);
	population.clear();
	for (int i = 0; i < n; ++i)
	{
		population.push_back(GetRandomChromosome());
	}
}
void DisplayPopulation()
{
	for (int i = 0; i < population.size(); ++i)
	{
		DrawBits(population[i]);
	}
}

int Popcount(Chromosome c)
{
	int sum = 0;
	while (c)
	{
		sum += (c & 1);
		c = (c >> 1);
	}
	return sum;
}
int FitnessFunction(Chromosome chromosome)
{
	Chromosome mask = 0x8 + 0x4 + 0x2 + 1;
	return Popcount(~(chromosome ^ mask));
}

void StepEra(int nRemains)
{
	std::sort(population.begin(), population.end(),
		[](Chromosome c1, Chromosome c2)
		{
			return FitnessFunction(c1) > FitnessFunction(c2);
		});

	for (int i = nRemains; i < population.size(); ++i)
	{
		int id1 = rand() % nRemains;
		int id2 = rand() % nRemains;

		Chromosome c = Crossover(population[0], population[1]);

		/// flip random bit
		int point = rand() % (sizeof(Chromosome) * 8);
		c = c ^ (1 << point);

		population[i] = c;
	}
}


int main()
{
	srand(time(nullptr));
	InitPopulation(5);
	DisplayPopulation();

	while (true)
	{
		StepEra(2);
		cout << "\n\\\\\\\n";
		DisplayPopulation();
		system("pause");
	}

	system("pause");
}