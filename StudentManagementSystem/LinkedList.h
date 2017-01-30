#pragma once
#include <exception>
#include <iterator>

class IllegalIndex : public std::exception
{

};

// interface for LinkedList

template <typename T>
class LinkedList
{
public:
    virtual ~LinkedList() = 0;
    // delete elements
    virtual T pop_front() = 0;
    virtual T pop_back() = 0;
    // add elements
    virtual void push_back(T obj) = 0;
    virtual void push_front(T obj) = 0;
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
    // iterators definition
    class LinkedList<T>::iterator
    {
    public:

        iterator(LinkedList& lk_list, int index)
            : index(index),
              lkList(lk_list)
        {
        }

        iterator& operator++()
        {
            index++;
            return *this;
        }

        iterator& operator--()
        {
            index--;
            return *this;
        }

        T operator*()
        {
            return at(index);
        }
    private:
        int index;
        LinkedList& lkList;
    };

    class LinkedList<T>::reverse_iterator
    {
    public:
        reverse_iterator(LinkedList& lk_list, int index)
            : index(lk_list.size() - index),
              lkList(lk_list)
        {
        }

        reverse_iterator& operator++()
        {
            index--;
            return *this;
        }

        reverse_iterator& operator--()
        {
            index++;
            return *this;
        }

        T operator*()
        {
            return at(index);
        }
    private:
        int index;
        LinkedList& lkList;
    };

    // iterators
    virtual iterator begin()
    {
        return iterator(this, 0);
    }
    virtual iterator end()
    {
        return iterator(this, size());
    }
    virtual reverse_iterator rbegin()
    {
        return reverse_iterator(this, 0);
    }
    virtual reverse_iterator rend()
    {
        return reverse_iterator(this, size());
    }

};

template <typename T>
LinkedList<T>::~LinkedList()
{
    // keep it empty, this abstract class does not need deconstructor
}

