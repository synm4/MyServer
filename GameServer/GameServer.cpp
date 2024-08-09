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

class Player
{
public:
	Player() {}
	virtual ~Player() {}
};

class Knight : public Player
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
	
	// 기존 stompallocator 에서 4kb로 페이징을 하고있는데, 할당한 영역에 비해
	// 실제 사용되는 영역이 터무니없이 작기 때문에 초과되는 영역의 접근을 막을 수 없음.
	// 오버플로우 문제를 막기위해 메모리 할당영역의 끝단으로 생성하도록 변경하여 해결. (언더플로우는 사실 없다고 봐도 무방)
	Knight* knight = (Knight*)xnew<Player>();

	knight->_hp = 100;

	xdelete(knight);

	

}