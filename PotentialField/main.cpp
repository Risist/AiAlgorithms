#include <iostream>
#include <cmath>
#include <vector>
#include "Vector.h"
using namespace std;


struct Obstacle
{
	Vector2 position;
	float influence = 1.0f;
};

vector<Obstacle> obstacles;

Vector2 GetForceAttraction(Vector2 goal)
{
	Vector2 attraction;
	for (auto& it : obstacles)
	{
		Vector2 toGoal = goal - it.position;
		float distanceSq = toGoal.GetLengthSq();

		attraction += toGoal/distanceSq * it.influence; /// Normalize and mltp by 1/length -> the closer the bigger force influence
	}

	return attraction;
}


int main()
{
	obstacles.push_back({ {0,5}, 1 });
	obstacles.push_back({ {1,2}, 1 });

	obstacles.push_back({ {-1,0}, 2 });

	const float movementSpeed = 0.5f;
	
	Vector2 myPosition;
	while (true)
	{
		Vector2 attraction = GetForceAttraction(myPosition);
		cout << "\n";
		cout << "Position = " << myPosition.x << ", " << myPosition.y << "\n";
		cout << "Attraction = " << attraction.x << ", " << attraction.y << "\n";
		//cout << "Attraction Length = " << attraction.GetLength() << "\n";
		if(attraction.GetLengthSq() > 0.25f*0.25f)
			myPosition += attraction.GetNormalized() * movementSpeed;

		system("pause");
	}
}