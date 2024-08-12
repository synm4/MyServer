#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "RefCounting.h"
#include "Memory.h"
#include "Allocator.h"
#include "LockFreeStack.h"

class Data // : public SlistEntry
{
public:
	SListEntry _entry;

	int32 _hp;
	int32 _mp;
};

void InitializeHead(SListHeader* header)
{
	header->next = nullptr;
}

void PushEntrySList(SListHeader* header, SListEntry* entry)
{
	entry->next = header->next;
	header->next = entry;
}

SListEntry* PopEntrySList(SListHeader* header)
{
	SListEntry* first = header->next;

	if (first != nullptr)
		header->next = first->next;

	return first;
}

int main()
{
	SListHeader header;
	InitializeHead(&header);

	Data* data = new Data();
	data->_hp = 10;
	data->_mp = 20;
	PushEntrySList(&header, (SListEntry*)data);

	Data* popData = (Data*)PopEntrySList(&header);

}