#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ConcurrentQueue.h"
#include "ConcurrentStack.h"

LockQueue<int32> q; 
LockStack<int32> s;

void Push()
{
	while (true)
	{
		int32 value = rand() % 100;
		q.Push(value);

		this_thread::sleep_for(10ms);
	}
}

void Pop()
{
	while (true)
	{
		int32 data = 0;
		//q.WaitPop(OUT data);  사용예시.
		if (q.TryPop(OUT data))
			cout << data << endl;
	}
}

int main(){
	thread t1(Push);
	thread t2(Pop);
	thread t3(Pop); // 일부러 push 보다 pop을 의도적으로 많이 수행하여 crash 유도
	
	t1.join();
	t2.join();
	t3.join();
}
