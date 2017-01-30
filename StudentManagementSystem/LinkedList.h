#pragma once
#include "LkListNode.h"

template <typename T>
class LinkedList
{
public:
    LinkedList()
    {
        head = new LkListNode<T>();
    }

    ~LinkedList()
    {
        LkListNode<T>* node = head;
        while (node) {
            LkListNode<T>* next = node->GetNext();
            delete node;
            node = next;
        }
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
    LkListNode<T>* GetHead() const
    {
        return head;
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
    bool Reverse()
    {
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
    LinkedList<T>& Join_InPlace(LinkedList<T>& other)
    {
        LkListNode<T>* otherHead = other.GetHead();
        LkListNode<T>* curListRear = GetNode(GetLength() - 1);

        curListRear->SetNext(otherHead->GetNext());
        other.GetHead()->SetNext(nullptr);

        return *this;
    }

    void push_back(T obj)
    {
        Insert(obj);
    }
    void shrink_to_fit()
    {

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

};

