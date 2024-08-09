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

class Knight
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	Knight(int32 hp) : _hp(hp)
	{
		cout << "Knight(hp)" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	int32 _hp = 100;
	int32 _mp = 10;
};

int main()
{
	// new 와 delete 에서는 무조건 메모리를 날리지않고 힙영역을 유동적으로 관리를함. 
	// 해제한 메모리 접근시 크래시가 날 수 도있고 안날수도있음.
	// winapi 인 VirtualAlloc 은 운영체제에게 아예 해당 메모리를 해제해달라고 요청하기때문에 
	// 해제한 메모리 접근시 바로 크래시가남.
	
	// CoreMacro.h 에서 BaseAllocator로 바꿔보면 메모리 오염에대한 크래시를 잡지 못하는것을 알 수 있음.
	Knight* knight = xnew<Knight>(100);

	xdelete(knight);

	knight->_hp = 100;

}