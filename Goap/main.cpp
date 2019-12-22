#include "GOAP.h"
#include "Action.h"



struct ActionDrawText : public Action
{
	ActionDrawText(const char* c) : _c(c)
	{
	}

	virtual void BeginExectution() override
	{
		std::cout << _c << "\n";
	}

private:
	const char* _c;
};

int main()
{
	Test_WorldState();
	ActionManager manager;

	manager.AddAction(new ActionDrawText(":P"), ESleaping, EDying, 2);
	manager.AddAction(new ActionDrawText(":)"), EWorking, EFighting, 1);
	manager.AddAction(new ActionDrawText(":<"), EFighting, EDying, 1);



	manager.worldState = { 0, 15 };
	manager.worldState.SetBit(2);
	manager.worldState.SetBit(0);
	manager.goal = { EDying };

	while (true)
	{
		system("pause");
		manager.Update();
	}

	system("pause");
}