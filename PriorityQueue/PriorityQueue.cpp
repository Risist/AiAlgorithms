#pragma once
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

using namespace std;

struct DataType
{
	int key;
	std::string value;

	bool operator<(const DataType& other) const { return key < other.key; }
	bool operator>(const DataType& other) const { return key > other.key; }
};

class PriorityQueueMin
{
public:
	void Allocate(int size)
	{
		delete _data;
		_data = new DataType[size];
		_heapSize = 0;
		_maxLength = size;
	}

	bool Empty() const
	{
		return _heapSize == 0;
	}
	std::string Top() const
	{
		return _data[0].value;
	}
	void Pop()
	{
		assert(!Empty());
		_data[0] = _data[--_heapSize];
		Heapify(0);
	}
	void Insert(std::string value, int key)
	{
		assert(_heapSize < _maxLength);

		_data[_heapSize].value = value;
		DecreaseKey(_heapSize++, key);
	}
	void DecreaseKey(int i, int newKey)
	{
		//assert(newKey <= _data[i].key);
		_data[i].key = newKey;
		while (i != 0 && _data[i] < _data[Parent(i)])
		{
			swap(_data[i], _data[Parent(i)]);
			i = Parent(i);
		}
	}
private:
	inline int Left(int i)
	{
		return (i << 1) + 1;
	}
	inline int Right(int i)
	{
		return (i << 1) + 2;
	}
	inline int Parent(int i)
	{
		return (i >> 1);
	}

	void Heapify(int i)
	{
		int l = Left(i);
		int r = Right(i);

		int smallest = i;
		if (l < _heapSize && _data[l] < _data[smallest])
			smallest = l;
		if (r < _heapSize && _data[r] < _data[smallest])
			smallest = r;

		if (smallest != i)
		{
			swap(_data[i], _data[smallest]);
			Heapify(smallest);
		}
	}

	DataType* _data = nullptr;
	int _maxLength = 0;
	int _heapSize = 0;
};

int main()
{
	PriorityQueueMin q;
	q.Allocate(100);
	q.Insert("5", 5);
	q.Insert("2", 2);
	q.Insert("3", 3);
	q.Insert("1", 1);
	q.Insert("4", 4);

	//q.Insert("4", 4);
	//q.Insert("3", 3);
	//q.Insert("2", 2);
	//q.Insert("1", 1);

	cout << q.Top() << "\n";
	system("pause");
}