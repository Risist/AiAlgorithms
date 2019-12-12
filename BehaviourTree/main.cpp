/// Project based on : http://www.gameaipro.com/GameAIPro/GameAIPro_Chapter06_The_Behavior_Tree_Starter_Kit.pdf
/// Done to learn how to implement Behaviour Tree in a good way

#include <iostream>
#include <stdint.h>
#include <vector>
#include "BehaviourTree6.h"
using namespace std;

class BehaviourLog : public Behaviour
{
public:
	BehaviourLog(const char* _txt)
		:_txt(_txt)
	{

	}

protected:
	virtual EStatus OnExecution() override
	{
		std::cout << _txt << "\n";
		return EStatus::ESuccess;
	}

private:
	const char* _txt;
};

class BehaviourConst : public Behaviour
{
public:
	BehaviourConst(EStatus _returnStatus)
		:_returnStatus(_returnStatus)
	{

	}

protected:
	virtual EStatus OnExecution() override
	{
		return _returnStatus;
	}

private:
	EStatus _returnStatus;
};

class BehaviourConstSeq : public Behaviour
{
public:
	BehaviourConstSeq(EStatus *_returnStatus, int size)
		:_returnStatus(_returnStatus), _size(size) {}
	~BehaviourConstSeq() { delete[] _returnStatus; }

protected:
	virtual void OnBeginExecution() override
	{
		_iterator = -1;
	}
	virtual EStatus OnExecution() override
	{
		_iterator = (_iterator + 1) % _size;
		return _returnStatus[_iterator];
	}

private:
	
	EStatus* _returnStatus;
	int _iterator;
	int _size;
};

class DecoratorLog : public Decorator
{
public:
	DecoratorLog(Behaviour* child, const char* _txt)
		:_txt(_txt), Decorator(child) {}

	virtual EStatus OnExecution() override
	{
		std::cout << _txt << "\n";
		return Decorator::OnExecution();
	}


private:
	const char* _txt;
};


int main()
{
	BehaviourTree tree(100000);

	
	auto& root = tree.Allocate<Sequence>();
	auto& paralel = tree.Allocate<Parallel>(Parallel::ERequireAll, Parallel::ERequireOne);

	
	//root.AddChild(tree.Allocate<BehaviourLog>(":)"));
	//root.AddChild(tree.Allocate<BehaviourLog>(":|"));
	root.AddChild(paralel);
	paralel.AddChild(tree.Allocate<BehaviourLog>("haha"));
	paralel.AddChild(tree.Allocate<BehaviourLog>("haha"));
	

	auto& d = tree.Allocate<BehaviourConstSeq>(new EStatus[2]{ EStatus::ERunning, EStatus::ESuccess }, 2);
	paralel.AddChild(tree.Allocate<DecoratorLog>(&d, "Decorator: :D"));
	//paralel.AddChild(tree.Allocate<BehaviourLog>("hahaHuj"));

	//root.AddChild(tree.Allocate<BehaviourLog>(":("));
	//root.AddChild(tree.Allocate<BehaviourConstSeq>(new EStatus[2]{ EStatus::ERunning, EStatus::ESuccess }, 2));
	//root.AddChild(tree.Allocate<BehaviourLog>(":<"));
	//root.AddChild(tree.Allocate<BehaviourLog>("Natalia to suka"));

	tree.SetRoot(root);
	while (true)
	{
		tree.Execute();
		system("pause");
	}
}