#pragma once

template <typename T>
class LkListNode
{
public:
    LkListNode(T data)
    {
        SetData(data);
        SetNext(nullptr);
    }

    LkListNode()
    {
        SetNext(nullptr);
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
private:
    T data;
    LkListNode* next = nullptr;
    //LkListNode* prior = nullptr;
};