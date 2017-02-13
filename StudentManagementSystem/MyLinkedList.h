#pragma once
#include "LkListNode.h"
#include "LinkedList.h"
#include "FileHandler.h"

template <typename T>
class MyLinkedList /* : public LinkedList<T>*/
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
    int size() const
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

    T pop_front()
    {
        T val = front();
        Destroy(0);
        return val;
    }
    T pop_back()
    {
        T val = back();
        Destroy(size() - 1);
        return val;
    }
    void push_front(T obj)
    {
        LkListNode<T>* node = new LkListNode<T>(obj);
        if (!node)
            return;

        node->SetNext(head->GetNext());
        head->SetNext(node);
    }

    T front() const
    {
        return GetData(0);
    }
    T back() const
    {
        return GetData(size() - 1);
    }
    void resize(int newsize)
    {
        // do nothing
    }

    // copy and move operator
    // Note: should do a deep copy
    MyLinkedList(const MyLinkedList& other)
    {
        head = new LkListNode<T>();
        for (auto iter = other.begin(); iter != other.end(); ++iter) {
            push_back(*iter);
        }
    }

    MyLinkedList(MyLinkedList&& other) noexcept
    {
        head = new LkListNode<T>();
        for (auto iter = other.begin(); iter != other.end(); ++iter) {
            push_back(*iter);
        }
    }

    MyLinkedList& operator=(const MyLinkedList& other)
    {
        if (this == &other)
            return *this;

        for (auto iter = other.begin(); iter != other.end(); ++iter) {
            push_back(*iter);
        }

        return *this;
    }

    MyLinkedList& operator=(MyLinkedList&& other) noexcept
    {
        if (this == &other)
            return *this;

        *this = other;

        return *this;
    }

    // iterators definition
    class mylklist_iterator /*: public LinkedList<T>::iterator*/
    {
    public:
        mylklist_iterator(MyLinkedList& lk_list, LkListNode<T>* node)
            : nodeptr(node),
              lkList(lk_list)
        {
        }

        mylklist_iterator& operator++()
        {
            nodeptr = nodeptr->GetNext();
            return *this;
        }

        mylklist_iterator& operator--()
        {
            nodeptr = nodeptr->GetPrev();
            return *this;
        }

        mylklist_iterator operator+(int val)
        {
            mylklist_iterator new_iter = *this;
            new_iter += val;
            return new_iter;
        }

        mylklist_iterator& operator+=(int val)
        {
            while (val) {
                ++(*this);
                val--;
            }
            return *this;
        }

        mylklist_iterator operator-(int val)
        {
            mylklist_iterator new_iter = *this;
            new_iter -= val;
            return new_iter;
        }

        mylklist_iterator& operator-=(int val)
        {
            while (val) {
                --(*this);
                val--;
            }
            return *this;
        }

        LkListNode<T>* GetNodePtr()
        {
            return nodeptr;
        }

        T operator*()
        {
            return nodeptr->GetData();
        }

        bool operator==(mylklist_iterator& other)
        {
            return this->nodeptr == other.nodeptr
                   && &(this->lkList) == &(other.lkList);
        }

        bool operator!=(mylklist_iterator& other)
        {
            return !(*this == other);
        }
    private:
        LkListNode<T>* nodeptr;
        MyLinkedList<T>& lkList;
    };

    class mylklist_reverse_iterator : public mylklist_iterator
    {
    public:
        mylklist_iterator& operator++()
        {
            mylklist_iterator::nodeptr = mylklist_iterator::nodeptr->GetPrev();
            return *this;
        }

        mylklist_iterator& operator--()
        {
            mylklist_iterator::nodeptr = mylklist_iterator::nodeptr->GetNext();
            return *this;
        }
    };

    void insert(mylklist_iterator& iter, T obj)
    {
        LkListNode<T>* node = new LkListNode<T>(obj);
        if (!node)
            return;

        mylklist_iterator prev_iter = iter - 1;

        LkListNode<T>* priorNode = prev_iter.GetNodePtr();

        node->SetNext(priorNode->GetNext());
        priorNode->SetNext(node);
    }

    mylklist_iterator erase(mylklist_iterator& where)
    {
        mylklist_iterator next_iter = where + 1;
        delete &(*where);
        return next_iter;
    }

    // iterators
    mylklist_iterator begin()
    {
        return mylklist_iterator(*this, head->GetNext());
    }

    mylklist_iterator end()
    {
        return mylklist_iterator(*this, nullptr);
    }

    mylklist_reverse_iterator rbegin()
    {
        return mylklist_reverse_iterator(*this, GetNode(size() - 1));
    }

    mylklist_reverse_iterator rend()
    {
        if (size() == 0)return rbegin();
        return mylklist_reverse_iterator(*this, head);
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
            node->SetPrev(priorNode);
            priorNode->SetNext(node);
        } else { // insert at head
            node->SetNext(head->GetNext());
            node->SetPrev(head);
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