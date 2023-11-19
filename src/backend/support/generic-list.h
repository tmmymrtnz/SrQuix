#ifndef GENERIC_LIST_H
#define GENERIC_LIST_H

// Define a node structure for the list
typedef struct Node {
    void* data;
    struct Node* next;
} Node;

// Define a list structure
typedef struct {
    Node* head;
    int size;
} List;

// Function to initialize a list
List* initList();

// Function to add an element to the list
int addElement(List* list, void* data);

// Function to find an element in the list
void* findElement(List* list, void* target, int (*compare)(const void*, const void*));

// Function to print the list
void printList(List* list, void (*printElement)(const void*));

// Function to free the list and its elements
void freeList(List* list);

// Function to get the size of the list
int getListSize(List* list);

#endif // GENERIC_LIST_H
