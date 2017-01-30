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

#pragma once
#include "LinkedList.h"

    // iterators definition
    // TODO: refactor this to fit LinkedList
    class iterator
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

        iterator& operator+(int val)
        {
            iterator new_iter = this;
            new_iter += val;
            return new_iter;
        }

        iterator& operator+=(int val)
        {
            index += val;
            return *this;
        }

        iterator& operator-(int val)
        {
            iterator new_iter = this;
            new_iter -= val;
            return new_iter;
        }

        iterator& operator-=(int val)
        {
            index -= val;
            return *this;
        }

        iterator(const iterator& other)
            : index(other.index),
              lkList(other.lkList)
        {
        }

        iterator(iterator&& other) noexcept
            : index(other.index),
              lkList(other.lkList)
        {
        }

        iterator& operator=(const iterator& other)
        {
            if (this == &other)
                return *this;
            index = other.index;
            lkList = other.lkList;
            return *this;
        }

        iterator& operator=(iterator&& other) noexcept
        {
            if (this == &other)
                return *this;
            index = other.index;
            lkList = other.lkList;
            return *this;
        }

        friend bool operator==(const iterator& lhs, const iterator& rhs)
        {
            return lhs.index == rhs.index
                   && lhs.lkList == rhs.lkList;
        }

        friend bool operator!=(const iterator& lhs, const iterator& rhs)
        {
            return !(lhs == rhs);
        }

        T operator*()
        {
            return at(index);
        }
    private:
        int index;
        LinkedList& lkList;
    };

    class reverse_iterator
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

    // delete elements
    virtual T pop_front() = 0;
    virtual T pop_back() = 0;
    virtual iterator erase(iterator where) = 0;
    virtual void erase(iterator first, iterator last)
    {
        for (iterator iter = first; iter != last; /*leave here blank because erase will return next iter*/) {
            iter = erase(iter);
        }
        erase(last);
    }
    virtual void erase(int pos)
    {
        iterator iter = begin();
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
    virtual void insert(iterator iter, T obj) = 0;
    virtual void insert(int index, T obj)
    {
        iterator iter = begin();
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
    virtual iterator begin()
    {
        return iterator(*this, 0);
    }
    virtual iterator end()
    {
        return iterator(*this, size());
    }
    virtual reverse_iterator rbegin()
    {
        return reverse_iterator(*this, 0);
    }
    virtual reverse_iterator rend()
    {
        return reverse_iterator(*this, size());
    }
};

template <typename T>
LinkedList<T>::~LinkedList()
{
    // keep it empty, this abstract class does not need deconstructor
}
