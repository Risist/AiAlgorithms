#pragma once
#include "GOAP.h"

struct Action
{
	// called when the action has been choosen to be executed
	virtual void BeginExectution() {}

	// called every frame when action has been choosen to be executed
	// returns true if action finished
	virtual bool Execute() { return true; }

};

struct ActionManager
{
	~ActionManager()
	{
		for (auto it : actions)
			delete it;
	}

	// performs actions according to plan
	// if plan has been completed tries to find a new one
	void Update();
	// performs search for a new plan
	inline void FindNewPlan();

	// inserts a new action that could potentially be executed
	inline void AddAction(Action* action, WorldState preconditions, WorldState effects, CostType cost);

private:

	inline void SetCurrentAction(Action* a)
	{
		currentAction = a;
		a->BeginExectution();
	}

	// 
	Action* currentAction = nullptr;

	// current part of plan
	ReferenceType currentId = -1;

	// plan is executed from last to first - goap planner returns plan in reversed order
	Plan plan;

	// actions agent can perform
	std::vector<Action*> actions;

	// action planner;
	GOAP goap;

public:

	// world state agent is currently in
	WorldState worldState;

	// desired world state
	WorldState goal;
};


inline void ActionManager::AddAction(Action* action, WorldState preconditions, WorldState effects, CostType cost)
{
	actions.push_back(action);
	goap.AddAction(static_cast<ReferenceType>(actions.size() - 1), preconditions, effects, cost);
}

inline void ActionManager::FindNewPlan()
{
	if (goap.FindPlan(plan, worldState, goal))
	{
		currentId = static_cast<ReferenceType>(plan.size()) - 1;
		const ReferenceType actionid = plan[currentId];
		SetCurrentAction(actions[actionid]);
	}
}