#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../support/generic-list.h"
#include "abstract-syntax-tree.h"

typedef struct symbol_t {
    //listas
    List * components;
    List * nodes;
} symbol_t;

typedef enum {
    COMPONENT_TYPE,
    NODE_TYPE
} object_type;

typedef struct component_t component_t;

typedef struct node_t node_t;

struct component_t {
    char *component_name;
    int constant;

    ComponentType * component_type;

    object_type prev_type;
    void *prev;

    object_type next_type;
    void * next;
};

struct node_t {
    char *name;

    object_type dir_type[4];
    void *dir[4];
};

typedef struct error_t error_t;

struct error_t {
    char *error_message;

    // in the future we could add the component where the error happened
};

//inicia la tabla de simbolos
symbol_t * symbolTableInit();

void addComponent(ComponentDefRec * component, ComponentType * component_type);

void addComponents(ComponentDefRec * component, ComponentType * component_type);

void addNode(DeclareNode * node);

void addNodes(DeclareNode * node);

void concatTo(char * fromObjectName, char * toObjectName);

void concatNodes(char * fromNodeName, char * toNodeName);

symbol_t * getSymbolTable();

void printErrors();

int hasErrors();

int checkUnlinked();

void printUnlinked();

void symbolTableFree();


#endif