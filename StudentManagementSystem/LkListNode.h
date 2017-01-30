#pragma once

template <typename T>
class LkListNode
{
public:
    LkListNode(T data)
    {
        SetData(data);
    }

    LkListNode()
    {
    }

    ~LkListNode()
    {
    }

    T GetData() const
    {
        return data;
    };
    void SetData(T data)
    {
        LkListNode::data = data;
    };
    LkListNode* GetNext() const
    {
        return next;
    };
    void SetNext(LkListNode* next)
    {
        LkListNode::next = next;
    }
    LkListNode* GetPrev() const
    {
        return prev;
    };
    void SetPrev(LkListNode* prev)
    {
        LkListNode::prev = prev;
    }
    LkListNode& next_node()
    {
        return *next;
    }
    LkListNode& prev_node()
    {
        return *prev;
    }
private:
    T data;
    LkListNode* next = nullptr;
    LkListNode* prev = nullptr;
};