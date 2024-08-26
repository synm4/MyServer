#include "pch.h"
#include "Memory.h"
#include "MemoryPool.h"
/*------------
	Memory
-------------*/

Memory::Memory()
{
	int sizeSteps[] = { 32, 128, 256 };
	int maxSizes[] = { 1024, 2048, 4096 };
	int32 tableIndex = 0;
	int32 size = 0;

	for (int i = 0; i < 3; ++i)
	{
		for (; size <= maxSizes[i]; size += sizeSteps[i])
		{
			MemoryPool* pool = new MemoryPool(size);
			_pools.push_back(pool);

			while (tableIndex <= size)
			{
				_poolTable[tableIndex] = pool;
				tableIndex++;
			}
		}
	}
}

Memory::~Memory()
{
	for (MemoryPool* pool : _pools)
		delete pool;

	_pools.clear();
}

void* Memory::Allocate(int32 size)
{
	MemoryHeader* header = nullptr;
	const int32 allocSize = size + sizeof(MemoryHeader);

#ifdef _STOMP
	header = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(allocSize));
#else
	if (allocSize > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링 최대 크리르 벗어나면 일반 할당
		header = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(allocSize, SLIST_ALIGNMENT));
	}
	else
	{
		// 메모리 풀에서 꺼내온다
		header = _poolTable[allocSize]->Pop();
	}
#endif

	return MemoryHeader::AttachHeader(header, allocSize);
}

void Memory::Release(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

	const int32 allocSize = header->allocSize;
	ASSERT_CRASH(allocSize > 0);

#ifdef _STOMP
	StompAllocator::Release(header);
#else
	if (allocSize > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링 최대 크기를 벗어나면 일반 해제
		::_aligned_free(header);
	}
	else
	{
		// 메모리 풀에 반납
		_poolTable[allocSize]->Push(header);
	}
#endif
	
}
