#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../support/clist.h"
#include "abstract-syntax-tree.h"

typedef struct symbol_t {
    //listas
    CList  * components;
    CList  * nodes;
} symbol_t;

typedef enum {
    COMPONENT_TYPE,
    NODE_TYPE
} object_type;

typedef struct component_t component_t;

typedef struct node_t node_t;

struct component_t {
    ComponentType * component_type;
    char *component_name;
    int constant;

    object_type prev_type;
    union {
        const component_t *prev;
        const node_t *prev_node;
    };

    object_type next_type;
    union {
        const component_t *next;
        const node_t *next_node;
    };
};

struct node_t {
    char *name;

    object_type dir1_type;
    union {
        const node_t *dir1;
        const component_t *dir1_component;
    };

    object_type dir2_type;
    union {
        const node_t *dir2;
        const component_t *dir2_component;
    };

    object_type dir3_type;
    union {
        const node_t *dir3;
        const component_t *dir3_component;
    };

    object_type dir4_type;
    union {
        const node_t *dir4;
        const component_t *dir4_component;
    };
};


//inicia la tabla de simbolos
void symbolTableInit();

void addComponent(ComponentDefRec * component, ComponentType * component_type);

void addComponents(ComponentDefRec * component, ComponentType * component_type);

void addNode(DeclareNode * node);

void addNodes(DeclareNode * node);

void concatTo(char * fromObjectName, char * toObjectName);

symbol_t * getSymbolTable();

void symbolTableFree();


#endif