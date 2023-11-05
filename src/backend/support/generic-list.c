#include <stdio.h>
#include <stdlib.h>
#include "generic-list.h" // Include the header file

// Function to initialize a list
List* initList() {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        return NULL; // Memory allocation failed
    }
    list->head = NULL;
    return list;
}

// Function to add an element to the list
int addElement(List* list, void* data) {
    if (list == NULL) {
        return 0; // Invalid list
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return 0; // Memory allocation failed
    }
    newNode->data = data;
    newNode->next = list->head;
    list->head = newNode;
    return 1; // Success
}

// Function to find an element in the list
void* findElement(List* list, void* target, int (*compare)(const void*, const void*)) {
    if (list == NULL || target == NULL || compare == NULL) {
        return NULL; // Invalid input
    }

    Node* current = list->head;
    while (current != NULL) {
        if (compare(current->data, target) == 0) {
            return current->data;
        }
        current = current->next;
    }
    return NULL; // Element not found
}

// Function to print the contents of the list
void printList(List* list, void (*printElement)(const void*)) {
    if (list == NULL || printElement == NULL) {
        return; // Invalid input
    }

    Node* current = list->head;
    while (current != NULL) {
        printElement(current->data);
        current = current->next;
    }
}

// Function to free the list and its elements
void freeList(List* list) {
    if (list == NULL) {
        return;
    }

    Node* current = list->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}
