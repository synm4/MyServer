#pragma once
// ������� �̿��� ����.
// �����ϰ� pop�� �ϱ��� ���� ��� �ٸ� �������� ������ ����
// pop�� �����Ͽ� �������θ� ����.
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace std;

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
        _condVar.notify_one(); // ��������ϰ��ִ� �����尡 ������� ����
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

template<typename T>
class LockFreeStack
{
    struct Node
    {
        Node(const T& value) : data(value), next(nullptr)
        {

        }

        T data;
        Node* next;
    };

public:
    // 1) �� ��带 �����
    // 2) �� ����� next = head
    // 3) head = �� ���

    // [ ][ ][ ][ ][ ]
    // [head]
    void Push(const T& value)
    {
        Node* node = new Node(value);
        node->next = _head;

        /*if (_head == node->next) �Ʒ� compare_exchange�� �� ������ atomic�ϰ� ����.
        {
            _head = node;
            return true;
        }
        else
        {
            node->next = _head;
            return false;
        }*/

        while (_head.compare_exchange_weak(node->next, node) == false)
        {
            //node->next = _head;
        }

        // �� ���̿� ��ġ�� ���ϸ�?
        // _head = node;
        // �ݺ��ɰ�� ��������ƴϰ� ���̺�� ���°� �� �� ����.
    }

    // 1) head �б�
    // 2) head->next �б�
    // 3) head = head->next
    // 4) data �����ؼ� ��ȯ
    // 5) ������ ��带 ����

    bool TryPop(T& value)
    {
        ++_popCount;

        Node* oldHead = _head;

        /*if (_head == oldhead) �Ʒ� compare_exchange�� �� ������ atomic�ϰ� ����.
        {
            _head = oldHead->next;
            return true;
        }
        else
        {
            oldHead = _head;
            return false;
        }*/

        while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
        {
            //oldHead = _head;
        }

        if (oldHead == nullptr)
        {
            --_popCount;
            return false;
        }
        //Exception X
        value = oldHead->data;

        // ��� ���� ����
        //delete oldHead;

        TryDelete(oldHead);

        return true;
    }

private:
    void TryDelete(Node* oldHead)
    {
        if (_popCount == 1)
        {
            // �� ȥ�ڳ�?
            // �̿� ȥ���ΰ� ���� ����� �ٸ� �����͵鵵 �����غ���

            Node* node = _pendingList.exchange(nullptr);

            if (--_popCount == 0)
            {
                // ����� �����尡 ���� -> ���� ����
                // �߰��� �ٸ������� ���Կ�������. ������ �����ʹ� �и��ص� ����
                DeleteNodes(node);
            }
            else if (node)
            {
                // ���� ���������� �ٽ� ���� ����
                ChainPendingNodeList(node);
            }
            delete oldHead;
        }
        else
        {
            // �����ֳ�? �׷� ���� �������� �ʰ� ���� ���ุ ����
            ChainPendingNode(oldHead);
            --_popCount;
        }
    }

    void ChainPendingNodeList(Node* first, Node* last)
    {
        last->next = _pendingList;

        while (_pendingList.compare_exchange_weak(last->next, first) == false)
        {

        }
    }

    void ChainPendingNodeList(Node* node)
    {
        Node* last = node;
        while (last->next)
            last = last->next;

        ChainPendingNodeList(node, last);
    }

    void ChainPendingNode(Node* node)
    {
        ChainPendingNodeList(node, node);
    }

    static void DeleteNodes(Node* node)
    {
        while (node)
        {
            Node* next = node->next;
            delete node;
            node = next;
        }
    }

    // [ ][ ][ ][ ][ ]
    // [head]
    atomic<Node*> _head;

    atomic<uint32_t> _popCount = 0; // pop�� �������� ������ ����
    atomic<Node*> _pendingList; // ���� �Ǿ�� �� ���� (ù��° ���)
};
