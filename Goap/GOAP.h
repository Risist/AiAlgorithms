#pragma once
#include "Settings.h"
#include "WorldState.h"
#include <vector>

// type used for computing cost of executing behaviours
using CostType = float;

// plan composed of indexes to actions
using Plan = std::vector<ReferenceType>;

struct GOAP
{
	// Tries to formulate a new plan
	// succesfuly found plan is placed in @plan object in reversed order
	// returns false if can't satisfy goal starting from initial state
	bool FindPlan( Plan& plan, WorldState initial, WorldState goal);

	// inserts a new action type as a potential part of plan
	// returns id of new action added
	// referenceId -> reference to external data
	ReferenceType AddAction(ReferenceType referenceId, WorldState preconditions, WorldState effects, CostType cost);

	// changes cost of given action
	void UpdateCost(ReferenceType actionId, CostType newCost);

	// changes preconditions of a given action
	void UpdatePreconditions(ReferenceType actionId, WorldState newPreconditions);

	// changes effects of a given aciton
	void UpdateEffects(ReferenceType actionId, WorldState newEffects);

private:
	// Holds data usefull for search purposes
	struct ActionNode
	{ 
		// parent to this node e.g. the one from which the best route has been computed
		ReferenceType parentId;
		
		// current best cost to arrive at the node
		CostType g;

		
		// cost of performing the action
		CostType cost;

		// requirements for this action to start to work
		WorldState preconditions;

		// changes to the world state after performing this action
		WorldState effects;
	};
	std::vector<ActionNode> actionData;
	
	// array of the same size as action data
	// holds id's of external data actions points to
	std::vector<ReferenceType> actionReferences;
};

inline ReferenceType GOAP::AddAction(ReferenceType referenceId, WorldState preconditions, WorldState effects, CostType cost)
{
	ActionNode action;
	action.cost = cost;
	action.effects = effects;
	action.preconditions = preconditions;

	actionData.push_back(action);
	actionReferences.push_back(referenceId);
	return static_cast<ReferenceType>(actionData.size())-1;
}

inline void GOAP::UpdateCost(ReferenceType actionId, CostType newCost)
{
	actionData[actionId].cost = newCost;
}
inline void GOAP::UpdatePreconditions(ReferenceType actionId, WorldState newPreconditions)
{
	actionData[actionId].preconditions = newPreconditions;
}
inline void GOAP::UpdateEffects(ReferenceType actionId, WorldState newEffects)
{
	actionData[actionId].effects = newEffects;
}