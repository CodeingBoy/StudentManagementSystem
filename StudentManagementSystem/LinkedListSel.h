#pragma once
// select which linked list should be used

using namespace std;

#ifdef USE_STL
#include <vector>
#define LINKED_LIST vector
#else
#include "LinkedList.h"
#define LINKED_LIST LinkedList
#endif