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
#include "GOAPWorldState.h"

using namespace std;

class Action
{
public:

	void ResetForSearch()
	{
		g = numeric_limits<int>::max();
		parent = nullptr;
	}

	bool operator<(const Action& other) const
	{
		return g < other.g;
	}
	bool operator>(const Action& other) const
	{
		return g > other.g;
	}
	void DisplayName() const
	{
		cout << _name << "\n";
	}

public:
	std::string _name;
	Action* parent;
	CostType g = 0;
	CostType cost = 0;

	WorldState _preconditions;
	WorldState _effects;
};
vector<Action> actions;

/// although in original version search is in reversed order than in mine i think it doesn't matter that much
/// Actually if we start search from current world state and we cannot find any solution we can somehow measure solutions we've found and choose the best instead of returning failure
void Plan(WorldState initial, WorldState goal)
{
	struct Node {
		WorldState state;
		CostType g = 0;
		Action* action = nullptr;
	};
	auto cmp = [=](Node a1, Node a2) {
		return a1.g + a1.state.GetDistance(goal) > a2.g + a2.state.GetDistance(goal);
	};
	priority_queue<Node, std::vector<Node>, decltype(cmp) > open(cmp);

	for (int i = 0; i < actions.size(); ++i)
	{
		actions[i].ResetForSearch();
	}

	open.push({ initial, 0 });

	while (!open.empty())
	{
		auto current = open.top();
		open.pop();

		if (goal.Fulfils(current.state))
		{
			cout << "Found solution\n";
			Action* action = current.action;
			while (action)
			{
				action->DisplayName();
				action = action->parent;
			}
			return;
		}

		for (int i = 0; i < actions.size(); ++i)
			if (actions[i]._preconditions.Fulfils(current.state))
			{
				int cost = current.g + actions[i].cost;

				if (cost < actions[i].g)
				{
					actions[i].g = cost;
					actions[i].parent = current.action;
					open.push({
						current.state.GetAppliedChange(actions[i]._effects),
						cost,
						&actions[i]
						});
				}
			}
	}


	cout << "Solution not found\n";
}

void AddAction(std::string name, WorldState preconditions, WorldState effects, int cost)
{
	Action action;
	action._name = name;
	action.cost = cost;
	action._effects = effects;
	action._preconditions = preconditions;

	actions.push_back(action);
}
void InitActions()
{
	AddAction("haha1", ESleaping, EDying, 100);
	AddAction("haha2", EWorking, EFighting, 1);
	AddAction("haha3", EFighting, EDying, 1);
}

int main()
{
	InitActions();
	Plan(ESleaping | EWorking, EDying);

	system("pause");
}