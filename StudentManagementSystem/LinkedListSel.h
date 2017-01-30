#pragma once
// select which linked list should be used

using namespace std;

#ifdef USE_STL
#include <vector>
#define LINKED_LIST vector
#else
#include "MyLinkedList.h"
#define LINKED_LIST MyLinkedList
#endif