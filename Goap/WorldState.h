#include <iostream>
#include <cstdint>
#include <assert.h>

/// exemplary goals/preconditions/effects
/// dunno how to call that
enum EGoalFlags
{
	ENoFlag = 0,
	ESleaping = 1 << 0,
	EDying = 1 << 1,
	EWorking = 1 << 2,
	EFighting = 1 << 3,
};

/// using bits is efficient
/// in presentation on gdc someone who worked at shadow of mordor said 
///		that in their case they optimized it so their goap have been using only 10 states
/// so having 64 possible states should be enough for most purposes
using WorldFlags = uint64_t;

class WorldState
{
public:
	/// creates a new WorldState with by default no flag set
	WorldState(WorldFlags fSet = ENoFlag, WorldFlags fUnset = ENoFlag)
	{
		UnsetFlags(fUnset);
		SetFlags(fSet);
	}

	void SetFlags(WorldFlags f)
	{
		_flags = _flags | f;
		_mask = _mask | f;
	}
	void UnsetFlags(WorldFlags f)
	{
		_flags = _flags & (~f);
		_mask = _mask | f;
	}
	void SetBit(int i)
	{
		WorldFlags bit = (1ull << i);
		_flags = _flags | bit;
		_mask = _mask | bit;
	}
	void UnsetBit(int i)
	{
		WorldFlags bit = (1ull << i);
		_flags = _flags & (~bit);
		_mask = _mask | bit;
	}


	/// checks whether other fillfils requirements described by this world state
	bool IsFulfiledBy(const WorldState& other) const
	{
		WorldFlags f1 = _flags & _mask;
		WorldFlags f2 = other._flags & _mask;
		WorldFlags s = f1 ^ f2;
		WorldFlags r = !s;
		bool eq = (_mask & other._mask) == _mask;

		return r && eq;
	}

	/// returns owerwritten state by other
	WorldState GetAppliedChange(const WorldState& other) const
	{
		WorldState state;
		state._flags = (_flags & (~other._mask)) |
			(other._flags & other._mask);
		state._mask = _mask | other._mask;
		return state;
	}

	/// heuristic measure of distance
	/// how many states has to change in order to fullfil given goal
	int GetDistance(const WorldState& goal) const
	{
		WorldFlags d = (_flags & goal._mask) ^ (goal._flags & goal._mask);
		int sum = 0;
		while (d)
		{
			sum += d & 1;
			d = d >> 1;
		}

		return sum;
	}

	// displays all flags hold in the world state; mask doesn't influence
	void DisplayAllFlags() const
	{
		WorldFlags flags = _flags;
		WorldFlags mask = _mask;

		while (mask)
		{
			std::cout << (flags & 1);
			flags = flags >> 1;
			mask = mask >> 1;
		}
		std::cout << "\n";
	}

	// displays only those flags that are marked by mask
	void DisplayFlags() const
	{
		WorldFlags flags = _flags;
		WorldFlags mask = _mask;

		while (mask)
		{
			if (mask & 1)
				std::cout << (flags & 1) << "\n";
			flags = flags >> 1;
			mask = mask >> 1;
		}
	}

	// flags set in the world state; note not all flags are relevant
	WorldFlags _flags = 0;

	// which flags are relevant to this world state
	WorldFlags _mask = 0;
};

inline void WorldStateTest()
{
	WorldState initial = ESleaping | EWorking;
	WorldState goal = EDying;
	WorldState state = initial;

	assert(WorldState(EWorking).IsFulfiledBy(state));
	state = initial.GetAppliedChange(EFighting);
	assert(WorldState(EFighting).IsFulfiledBy(state));
	state = state.GetAppliedChange(EDying);
	assert(goal.IsFulfiledBy(state));
}
inline void WorldStateTest3()
{
	WorldState s1 = ESleaping | EWorking;
	WorldState s2 = ESleaping | EWorking;
	assert(s1.IsFulfiledBy(s2));
	assert(s2.IsFulfiledBy(s1));
}

inline void WorldStateTest4()
{
	WorldState s1 = ESleaping;
	WorldState s2 = EDying;
	assert(!s1.IsFulfiledBy(s2));
	assert(!s2.IsFulfiledBy(s1));
}
inline void WorldStateTest5()
{
	WorldState s1 = ESleaping | EWorking;
	WorldState s2 = ESleaping | EFighting;
	assert(!s1.IsFulfiledBy(s2));
	assert(!s2.IsFulfiledBy(s1));
}
inline void WorldStateTest6()
{
	WorldState s1 = ESleaping | EWorking;
	WorldState s2 = ESleaping;
	assert(!s1.IsFulfiledBy(s2));
	assert(s2.IsFulfiledBy(s1));
}
inline void WorldStateTest7()
{
	WorldState s1 = ESleaping | EWorking;
	WorldState s2 = ESleaping | EWorking | EFighting;
	assert(s1.IsFulfiledBy(s2));
	assert(!s2.IsFulfiledBy(s1));
}

inline void WorldStateTest2()
{
	WorldState s1 = { 0, EWorking };
	WorldState s2 = EWorking;
	assert(!s1.IsFulfiledBy(s2));
	assert(!s2.IsFulfiledBy(s1));
}
inline void WorldStateTest8()
{
	WorldState s1 = { 0, EWorking };
	WorldState s2 = ESleaping;
	assert(!s1.IsFulfiledBy(s2));
	assert(!s2.IsFulfiledBy(s1));
}
inline void WorldStateTest9()
{
	WorldState s1 = { 0, 15 };
	WorldState s2 = 0;
	assert(!s1.IsFulfiledBy(s2));
	assert(s2.IsFulfiledBy(s1));
}

inline void Test_WorldState()
{
	WorldStateTest();
	WorldStateTest2();
	WorldStateTest3();
	WorldStateTest4();
	WorldStateTest5();
	WorldStateTest6();
	WorldStateTest7();
	WorldStateTest8();
	WorldStateTest9();
}