#pragma once
#include <cstdint>
#include <assert.h>

class StackMemoryAllocator
{
public:
	using Offset_t = uint32_t;

	StackMemoryAllocator(Offset_t byteCount)
	{
		_buffer = new uint8_t[byteCount];
		_stackSize = byteCount;
	}
	~StackMemoryAllocator()
	{
		delete[] _buffer;
	}

	template<typename Ty, typename... Params>
	Ty& Allocate(Params... params)
	{
		Ty* n = new ((void*)((uintptr_t)_buffer + (uintptr_t)_offset))Ty(params...);
		_offset += sizeof(Ty);
		assert(_offset <= _stackSize, "allocation overflow");
		return *n;
	}
	template<typename Ty> Ty* AllocateArray(size_t n)
	{
		Ty* node = new ((void*)((uintptr_t)_buffer + (uintptr_t)_offset)) Ty[n];
		_offset += sizeof(Ty) * n;
		assert(_offset <= _stackSize, "allocation overflow");
		return node;
	}

	Offset_t GetOffset() { return _offset; }
	void SetOffset(Offset_t f) { _offset = f; }


private:
	uint8_t* _buffer = nullptr;
	Offset_t _offset = 0;
	Offset_t _stackSize = 0;
};