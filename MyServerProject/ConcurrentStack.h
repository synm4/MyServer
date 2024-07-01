#pragma once
// 락방식을 이용한 스택.
// 안전하게 pop을 하기전 락을 잡아 다른 쓰레드의 접근을 막고
// pop을 진행하여 성공여부를 리턴.
#include <mutex>

template<typename T>
class LockStack
{
public:
	LockStack() {}
	
	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_condVar.notify_one(); // 다음대기하고있는 쓰레드가 있을경우 깨움
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;
		
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] {return _stack.empty() == false; });
		value = std::move(_stack.top());
		_stack.pop();
	}

private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _condVar;
};

