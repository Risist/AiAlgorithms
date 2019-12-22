#include "Action.h"

void ActionManager::Update()
{
	if (currentId < 0)
	{
		FindNewPlan();
		return;
	}

	if (currentAction->Execute())
	{
		if (--currentId < 0)
		{
			FindNewPlan();
			return;
		}
		SetCurrentAction(actions[plan[currentId]]);
	}
}