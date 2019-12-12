#pragma once
#include <iostream>
#include <cstdint>
#include <assert.h>
#include <limits>
#include <cmath>

using namespace std;

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

/// using bits is efficient but from other hand very limited approach
using WorldFlags = uint64_t;
/// float is more suficient than int
/// our heuristic measure 1 for each flag that isn't meet so it leaves us not much room to balance things out
using CostType = float;

class WorldState
{
public:
	WorldState(WorldFlags fSet = EGoalFlags::ENoFlag, WorldFlags fUnset = EGoalFlags::ENoFlag)
	{
		SetFlags(fSet);
		SetFlags(fUnset);
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
	bool Fulfils(const WorldState& other) const
	{
		return !((_flags & _mask) ^ (other._flags & other._mask & _mask));
	}
	/// this state is a state to be overwritten by other
	WorldState GetAppliedChange(const WorldState& other) const
	{
		WorldState state;
		state._flags = (_flags & (~other._mask)) |
			(other._flags & other._mask);
		state._mask = _mask | other._mask;
		return state;
	}
	int GetDistance(const WorldState& other) const
	{
		WorldFlags d = (_flags & other._mask) ^ (other._flags & other._mask);
		int sum = 0;
		while (d)
		{
			sum += d & 1;
			d = d >> 1;
		}

		return sum;
	}


	void DisplayAllFlags() const
	{
		WorldFlags flags = _flags;
		WorldFlags mask = _mask;

		while (mask)
		{
			cout << (flags & 1) << "\n";
			flags = flags >> 1;
			mask = mask >> 1;
		}
	}

	void DisplayFlags() const
	{
		WorldFlags flags = _flags;
		WorldFlags mask = _mask;

		while (mask)
		{
			if (mask & 1)
				cout << (flags & 1) << "\n";
			flags = flags >> 1;
			mask = mask >> 1;
		}
	}

	//private:
	WorldFlags _flags = 0;
	WorldFlags _mask = 0;
};