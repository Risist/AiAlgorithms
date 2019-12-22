#include "GOAP.h"
#include <queue>

bool GOAP::FindPlan( Plan& plan, WorldState initial, WorldState goal)
{
	struct Node {
		WorldState state;
		CostType g;
		int actionId;
	};
	auto cmp = [=](Node a1, Node a2) {
		return a1.g + a1.state.GetDistance(goal) > a2.g + a2.state.GetDistance(goal);
	};
	std::priority_queue<Node, std::vector<Node>, decltype(cmp) > open(cmp);


	/// reset actions state
	for (int i = 0; i < actionData.size(); ++i)
	{
		actionData[i].g = std::numeric_limits<CostType>::max();
		actionData[i].parentId = -1;
	}
	open.push({ initial, 0, -1 });


	while (!open.empty())
	{
		auto current = open.top();
		open.pop();

		// check if goal has been found
		if (goal.IsFulfiledBy(current.state))
		{
			plan.clear();
			int id = current.actionId;
			while (id != -1)
			{
				plan.push_back(actionReferences[id]);
				id = actionData[id].parentId;
			}
			return true;
		}

		for (int i = 0; i < actionData.size(); ++i)
			if (actionData[i].preconditions.IsFulfiledBy(current.state))
			{
				CostType cost = current.g + actionData[i].cost;
				if (cost < actionData[i].g)
				{
					actionData[i].g = cost;
					actionData[i].parentId = current.actionId;
					open.push({ current.state.GetAppliedChange(actionData[i].effects),
						cost,
						i
						});
				}
			}
	}

	return false;
}