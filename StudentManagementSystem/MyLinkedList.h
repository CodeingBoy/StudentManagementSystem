#pragma once
#include "LkListNode.h"
#include "LinkedList.h"

template <typename T>
class MyLinkedList : public LinkedList<T>
{
public:
    MyLinkedList()
    {
        head = new LkListNode<T>();
    }

    ~MyLinkedList()
    {
        LkListNode<T>* node = head;
        while (node) {
            LkListNode<T>* next = node->GetNext();
            delete node;
            node = next;
        }
    }

    void push_back(T obj)
    {
        Insert(obj);
    }
    int size()
    {
        return GetLength();
    }
    T at(int pos)
    {
        if (pos < 0 || pos > size()) {
            throw IllegalIndex();
        }
        return GetData(pos);
    }

    T pop_front() override
    {
        T val = front();
        Destroy(0);
        return val;
    }
    T pop_back() override
    {
        T val = back();
        Destroy(size() - 1);
        return val;
    }
    void push_front(T obj) override
    {
        LkListNode<T>* node = new LkListNode<T>(obj);
        if (!node)
            return;

        node->SetNext(head->GetNext());
        head->SetNext(node);
    }

    T front() override
    {
        return GetData(0);
    }
    T back() override
    {
        return GetData(size() - 1);
    }
    void resize(int newsize) override
    {
        // do nothing
    }

    LinkedList<T>::iterator erase(LinkedList<T>::iterator where) override
    {
        LinkedList<T>::iterator next_iter = where + 1;
        delete &(*where);
        return next_iter;
    }
    void insert(LinkedList<T>::iterator iter, T obj) override
    {
        LkListNode<T>* node = new LkListNode<T>(obj);
        if (!node)
            return;

        LinkedList<T>::iterator prev_iter = iter - 1;

        LkListNode<T>* priorNode = (*prev_iter);

        node->SetNext(priorNode->GetNext());
        priorNode->SetNext(node);
    }
private:
    LkListNode<T>* head;

    LkListNode<T>* GetNode(int index)
    {
        LkListNode<T>* node = head->GetNext();
        int curIndex = 0;

        while (curIndex < index) {
            node = node->GetNext();
            curIndex++;
        }

        return node;
    }

    bool Insert(T data, int index = -1)
    {
        LkListNode<T>* node = new LkListNode<T>(data);
        if (!node)
            return false;

        if (index == -1)
            index = GetLength();

        if (index > 0) { // insert at other place
            LkListNode<T>* priorNode = GetNode(index - 1);

            node->SetNext(priorNode->GetNext());
            priorNode->SetNext(node);
        } else { // insert at head
            node->SetNext(head->GetNext());
            head->SetNext(node);
        }
        return true;
    }
    int GetLength() const
    {
        LkListNode<T>* node = head->GetNext();
        int curIndex = 0;

        while (node) {
            node = node->GetNext();
            curIndex++;
        }
        return curIndex;
    }
    bool IsEmpty() const
    {
        return head->next == nullptr ? true : false;
    }
    bool Destroy(int index)
    {
        delete GetNode(index);
        return false;
    }
    T GetData(int index)
    {
        LkListNode<T>* node = GetNode(index);

        if (node)
            return node->GetData();
        else
            return T();
    }
};

