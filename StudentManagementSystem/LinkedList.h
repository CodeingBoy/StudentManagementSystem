#pragma once
#include <exception>
#include <iterator>
#include "LinkedListSel.h"

class IllegalIndex : public std::exception
{

};

// interface for LinkedList

template <typename T>
class LinkedList
{
public:
    virtual ~LinkedList() = 0;

    // iterators definition
    class iterator
    {
    public:
        virtual ~iterator();

        // these should be override
        virtual iterator& operator++()
        {
            return nullptr;
        };
        virtual iterator& operator--()
        {
            return nullptr;
        };
        virtual iterator operator+(int val)
        {
            return nullptr;
        };
        virtual iterator& operator+=(int val)
        {
            return nullptr;
        };
        virtual iterator operator-(int val)
        {
            return nullptr;
        };
        virtual iterator& operator-=(int val)
        {
            return nullptr;
        };
        virtual bool operator==(iterator& other)
        {
            return false;
        };
        virtual bool operator!=(iterator& other)
        {
            return false;
        };
        virtual T operator*()
        {
            return T();
        };
    };

    // delete elements
    virtual T pop_front() = 0;
    virtual T pop_back() = 0;
    virtual iterator& erase(iterator& where) = 0;
    virtual void erase(iterator& first, iterator& last)
    {
        for (iterator& iter = first; iter != last; /*leave here blank because erase will return next iter*/) {
            iter = erase(iter);
        }
        erase(last);
    }
    virtual void erase(int pos)
    {
        iterator& iter = begin();
        iter += pos;
        erase(iter);
    };
    void clear()
    {
        erase(begin(), end());
    }
    // add elements
    virtual void push_back(T obj) = 0;
    virtual void push_front(T obj) = 0;
    virtual void insert(iterator& iter, T obj) = 0;
    virtual void insert(int index, T obj)
    {
        iterator& iter = begin();
        iter += index;
        insert(iter, obj);
    }

    // access elements
    virtual T front() = 0;
    virtual T back() = 0;
    virtual T at(int pos) = 0;
    // informations
    virtual int size() = 0;
    virtual void resize(int newsize) = 0;

    // operators
    T& operator[](const int pos)
    {
        return at(pos);
    }

    // iterators
    virtual iterator& begin() = 0;
    virtual iterator& end() = 0;
    virtual iterator& rbegin() = 0;
    virtual iterator& rend() = 0;
};

template <typename T>
LinkedList<T>::~LinkedList()
{
    // keep it empty, this abstract class does not need deconstructor
}

template <typename T>
LinkedList<T>::iterator::~iterator()
{
    // keep it empty, this abstract class does not need deconstructor
}
