#include "symbol-table.h"


symbol_t *symbolTable = NULL;


void symbolTableInit(){
    if(symbolTable != NULL){
        printf("Symbol table already initialized\n");
        return;
    }

    symbolTable = malloc(sizeof(symbol_t));

    symbolTable->components = CList_init(sizeof(component_t));
    symbolTable->nodes = CList_init(sizeof(node_t));
}

symbol_t * getSymbolTable(){
    return symbolTable;
}


void addComponent(ComponentDefRec * component, ComponentType * component_type) {
    component_t new_component;
    new_component.component_type = component_type;
    strcpy(new_component.component_name, component->component_name);
    new_component.constant = component->constant;

    // firstMatch to check if the component already exists
    component_t *existing_component = (component_t *)symbolTable->components->firstMatch(
        symbolTable->components, &new_component, 0, strlen(new_component.component_name), 1
    );

    // node_t new_node;
    // strcpy(new_node.name, component->component_name);

    // firstMatch to check if the node already exists
    node_t *existing_node;
    // node_t *existing_node = (node_t *)symbolTable->nodes->firstMatch(
    //     symbolTable->nodes, &new_node, 0, strlen(new_node.name), 1
    // );

    // If existing_component is NULL, the component does not exist
    if (existing_component == NULL && existing_node == NULL) {
        symbolTable->components->add(symbolTable->components, &new_component);
    } else {
        printf("[ERROR] Ya existia un objeto con ese nombre, banana!!!\n");
    }
}

void addComponents(ComponentDefRec * component, ComponentType * component_type) {
    ComponentDefRec * current_component = component;
    while (current_component != NULL) {
        addComponent(current_component, component_type);
        current_component = current_component->component_def_rec;
    }
}


void addNode(DeclareNode * node) {
    node_t new_node;
    strcpy(new_node.name, node->name);

    // firstMatch to check if the node already exists
    node_t *existing_node = (node_t *)symbolTable->nodes->firstMatch(
        symbolTable->nodes, &new_node, 0, strlen(new_node.name), 1
    );

    // component_t new_component;
    // strcpy(new_component.component_name, node->name);

    // firstMatch to check if the component already exists
    component_t *existing_component;
    // component_t *existing_component = (component_t *)symbolTable->components->firstMatch(
    //     symbolTable->components, &new_component, 0, strlen(new_component.component_name), 1
    // );

    // If existing_node is NULL, the node does not exist
    if (existing_node == NULL && existing_component == NULL) {
        symbolTable->nodes->add(symbolTable->nodes, &new_node);
    } else {
        printf("[ERROR] Ya existia un objeto con ese nombre, banana!!!\n");
    }
}

void addNodes(DeclareNode * node) {
    DeclareNode * current_node = node;
    while (current_node != NULL) {
        addNode(current_node);
        current_node = current_node->declare_node;
    }
}


void symbolTableFree(){
    if(symbolTable == NULL){
        printf("Symbol table already freed\n");
        return;
    }

    symbolTable->components->free(symbolTable->components);
    symbolTable->nodes->free(symbolTable->nodes);
    free(symbolTable);
    symbolTable = NULL;
}