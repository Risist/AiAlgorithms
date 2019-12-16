#include "Vector.h"
#include "Data.h"
#include "GradientDescent.h"


float DistanceToEndEffector(Vector2 target)
{
	return (target - GetEndEffector()).GetLength();
}

int main()
{
	//ComputeChainPositions();

	//DisplayChainAngles();
	//DisplayChainPositions();

	Vector2 target{ 1,1 };
	int i = 0;
	while (true)
	{

		InverseKinematicsStep_GradientDescent(target, 0.5f/(i+1));
		cout << "i = " << i <<", error = " << DistanceToEndEffector(target);
		

		system("pause");
		++i;
	}
}