#pragma once
// select which linked list should be used

using namespace std;

#ifdef USE_STL_LIST
#include <list>
#define LINKED_LIST list
#define LKLIST_NODEPTR _NODEPTR
#else
#include "MyLinkedList.h"
#define LINKED_LIST MyLinkedList
#define LKLIST_NODEPTR LkListNode
#endif