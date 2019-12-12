#pragma once
#include "Allocators.h"
#include <iostream>

enum class EStatus
{
	ESuccess,
	EFailure,

	ERunning,
};


class Behaviour
{
public:
	EStatus Execute()
	{
		return _status = OnExecution();
	}

public:
	EStatus GetStatus() const { return _status; }

	virtual void OnBeginExecution() {}
	virtual EStatus OnExecution() = 0;
private:
	EStatus _status = EStatus::ESuccess;
	void LogStatus()
	{
		switch (_status)
		{
		case EStatus::EFailure:
			std::cout << "EFailure\n";
			break;
		case EStatus::ESuccess:
			std::cout << "ESuccess\n";
			break;

		case EStatus::ERunning:
			std::cout << "ERunning\n";
			break;
		default:
			std::cout << "INVALID STATUS\n";
		}
	}
};
class BehaviourTree
{
public:
	BehaviourTree(int stackSize) : _allocator(stackSize) {}

	template<typename Ty>						Ty& Allocate()
	{
		return _allocator.Allocate<Ty>();
	}
	template<typename Ty, typename... Params>	Ty& Allocate(Params... params)
	{
		return _allocator.Allocate<Ty>(params...);
	}

	void SetRoot(Behaviour& behaviour) { _root = &behaviour; }
	void Execute() const
	{
		if(_root->GetStatus() != EStatus::ERunning)
			_root->OnBeginExecution();
		_root->Execute();
	}

private:
	StackMemoryAllocator _allocator;
	Behaviour* _root;
};


class Composite : public Behaviour
{
public:
	void AddChild(Behaviour& child)
	{
		assert(_childCount < MAX_CHILD_COUNT);
		ptrdiff_t p = (uintptr_t)&child - (uintptr_t)this;
		_children[_childCount++] = static_cast<uint32_t>(p);
	}
	void RemoveChild(int i)
	{
		assert(_childCount > 0);
		std::swap(_children[i], _children[_childCount - 1]);
		--_childCount;
	}
	void CleanChildren() {
		_childCount = 0;
	}

	uint8_t    GetChildCount() const { return _childCount; }
	Behaviour& GetChild(int i) const {
		return *(Behaviour*)((uintptr_t)this + (uintptr_t)_children[i]);
	}
private:
	static const unsigned int MAX_CHILD_COUNT = 7u;
	uint32_t _children[MAX_CHILD_COUNT];
	uint8_t _childCount = 0;
};

class Sequence : public Composite
{
public:
	virtual void OnBeginExecution() override
	{
		_iterator = 0;
		for (int i = 0; i < GetChildCount(); ++i)
			GetChild(i).OnBeginExecution();
	}
	virtual EStatus OnExecution() override
	{
		while (true)
		{
			auto& child = GetChild(_iterator);
			child.Execute();
			EStatus childStatus = child.GetStatus();

			if (childStatus != EStatus::ESuccess)
				return childStatus;
			else if (++_iterator >= GetChildCount())
				return EStatus::EFailure;
		}
	}
private:
	int _iterator = 0;
};

class Selector : public Composite
{
public:
	virtual void OnBeginExecution() override
	{
		_iterator = 0;
		for (int i = 0; i < GetChildCount(); ++i)
			GetChild(i).OnBeginExecution();
	}
	virtual EStatus OnExecution() override
	{
		while (true)
		{
			auto& child = GetChild(_iterator);
			child.Execute();
			EStatus childStatus = child.GetStatus();

			if (childStatus != EStatus::EFailure)
				return childStatus;
			else if (++_iterator >= GetChildCount())
				return EStatus::EFailure;
		}
	}
private:
	int _iterator = 0;
};

class Parallel : public Composite
{
public:
	enum EReturnPolicy
	{
		ERequireOne,
		ERequireAll,
	}; 
	Parallel(EReturnPolicy _policySuccess, EReturnPolicy _policyFailure)
		:_policySuccess(_policySuccess), _policyFailure(_policyFailure)
	{}

	virtual EStatus OnExecution() override
	{
		uint8_t successCount = 0;
		uint8_t failureCount = 0;
		uint8_t childCount = GetChildCount();
		EStatus returnStatus = EStatus::ERunning;
		for (int i = 0; i < childCount; ++i)
		{
			auto& child = GetChild(i);
			EStatus status = child.OnExecution();
			if (status == EStatus::ESuccess)
			{
				if (_policySuccess == EReturnPolicy::ERequireOne)
					returnStatus = EStatus::ESuccess;
				++successCount;
			}
			else if (status == EStatus::EFailure)
			{
				if (_policyFailure == EReturnPolicy::ERequireOne)
					returnStatus = EStatus::EFailure;
				++failureCount;
			}
		}
		if (_policyFailure == EReturnPolicy::ERequireAll && failureCount == childCount)
			return EStatus::EFailure;
		if (_policySuccess == EReturnPolicy::ERequireAll && successCount == childCount)
			return EStatus::ESuccess;
		return returnStatus;
	}
private:
	EReturnPolicy _policySuccess;
	EReturnPolicy _policyFailure;
};

class Decorator : public Behaviour
{
public:
	Decorator(Behaviour* child)
		: _child((uintptr_t)child - (uintptr_t)this)
	{}

public:
	virtual void OnBeginExecution() override
	{
		GetChild().OnBeginExecution();
	}
	virtual EStatus OnExecution() override
	{
		return GetChild().Execute();
	}

protected:
	Behaviour& GetChild() const
	{
		return *(Behaviour*)((uintptr_t)this + (uintptr_t)_child);
	}

private:
	uint32_t _child = 0;
};


//////////////////////////////
//// Test Behaviours and decorators
//// Used to check if everything works
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
	BehaviourConstSeq(EStatus* _returnStatus, int size)
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