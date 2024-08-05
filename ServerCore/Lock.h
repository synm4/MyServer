#pragma once
#include "Types.h"

/*--------------------------- 
		RW SpinLock
 99.999999999999% 일정					read
 0.0000000000001% 변경있을 수 있음		write
 거의 대부분의 상황에서는 락을 사용할 필요가 없는 상황이기때문에 Reader-Writer Lock 을 사용
 극악의 확률로 write 할경우에 상호베타적으로 한명만 통과하게 진행함
 
-----------------------------*/

/*--------------------------------------------
	[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
   내 쓰레드 ID를 기입. <> 몇명이 사용하고있는지 COUNT
W : WriteFlag (Exclusive Lock Owner Threadid)
R : ReadFlag (Shared Lock Count)
---------------------------------------------*/

// W -> W (O)
// W -> R (O)  락 잡는 순서
// R -> W (X)

class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMOUT_TICK = 10000,
		MAX_SPIN_COUNT = 5000,
		WRITE_THREAD_MASK = 0xFFFF'0000,
		READ_COUNT_MASK = 0x0000'FFFF,
		EMPTY_FLAG = 0x0000'0000
	};

public:
	void WriteLock(const char* name);
	void WriteUnlock(const char* name);
	void ReadLock(const char* name);
	void ReadUnlock(const char* name);

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;
};

/*--------------------------
		LockGuards
/*-------------------------*/

// RAii 패턴
class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.ReadLock(name); }
	~ReadLockGuard() { _lock.ReadUnlock(_name); }

private:
	Lock& _lock;
	const char* _name;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock, const char* name) : _lock(lock) , _name(name) { _lock.WriteLock(name); }
	~WriteLockGuard() { _lock.WriteUnlock(_name); }

private:
	Lock& _lock;
	const char* _name;
};