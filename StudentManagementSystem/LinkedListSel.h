#pragma once
// select which linked list should be used

#ifdef USE_STL
#define LINKED_LIST vector
#else
#define LINKED_LIST LinkedList
#endif