#include "pch.h"
#include "Allocator.h"

/*-------------------
	BaseAllocator
-------------------*/

void* BaseAllocator::Alloc(int32 size)
{
	return ::malloc(size);
}

void BaseAllocator::Release(void* ptr)
{
	::free(ptr);
}

/*-------------------
	StompAllocator
-------------------*/

void* StompAllocator::Alloc(int32 size)
{
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE; // 반올림코드 4095 
	const int64 dataOffset = pageCount * PAGE_SIZE - size; // [             |  ] 커서 = dataOffset
	// pageCount * PAGE_SIZE = 전체 영역
	void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
}

void StompAllocator::Release(void* ptr)
{
	const int64 address = reinterpret_cast<int64>(ptr);
	const int64 baseAdress = address - (address % PAGE_SIZE); // 앞에 비어있는공간
	::VirtualFree(reinterpret_cast<void*>(baseAdress), 0, MEM_RELEASE);
}
