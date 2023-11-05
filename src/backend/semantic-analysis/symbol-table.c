#include "symbol-table.h"


symbol_t *symbolTable = NULL;


void symbolTableInit(){
    if(symbolTable != NULL){
        printf("Symbol table already initialized\n");
        return;
    }

    symbol_t* symbol = (symbol_t*)malloc(sizeof(symbol_t));
    if (symbol == NULL) {
        return; // Memory allocation failed
    }
    symbol->components = initList();
    symbol->nodes = initList();

    symbolTable = symbol;
    return;
}

symbol_t * getSymbolTable(){
    return symbolTable;
}

int compareComponentByName(const void* a, const void* b) {
    const struct component_t* component_a = (const struct component_t*)a;
    const struct component_t* component_b = (const struct component_t*)b;
    
    // Compare component names using strcmp
    return strcmp(component_a->component_name, component_b->component_name);
}

int compareNodeByName(const void* a, const void* b) {
    const struct node_t* node_a = (const struct node_t*)a;
    const struct node_t* node_b = (const struct node_t*)b;
    
    // Compare component names using strcmp
    return strcmp(node_a->name, node_b->name);
}

int check = 0;

// Function to print an element of type struct component_t
void printComponent(const void* data) {
    const struct component_t* component = (const struct component_t*)data;
    printf("Component Name: %s, Constant: %d\n", component->component_name, component->constant);
}

// Function to print an element of type struct component_t
void printNode(const void* data) {
    const struct node_t* node = (const struct node_t*)data;
    printf("Node Name: %s\n", node->name);
}

int objectExists(char * name) {
    component_t component_search; // Create a search object
    component_search.component_name = (char*)malloc(strlen(name) + 1);
    strcpy(component_search.component_name, name);

    check++;

    // firstMatch to check if the component already exists
    component_t* existing_component = (component_t*)findElement(symbolTable->components, &component_search, compareComponentByName);

    printList(symbolTable->components, printComponent);
    printf("[CF?] %s\n", existing_component == NULL? "componente vacio": "componente econtrado");

    if ( existing_component != NULL ) {
        printf("[ERROR] Ya existia un componente con ese nombre, banana!!!\n");
        return 1;
    }
    
    node_t node_search; // Create a search object
    node_search.name = (char*)malloc(strlen(name) + 1);
    strcpy(node_search.name, name);

    // firstMatch to check if the node already exists
    node_t* existing_node = (node_t*)findElement(symbolTable->nodes, &node_search, compareNodeByName);

    printList(symbolTable->nodes, printNode);
    printf("[NF?] %s\n", existing_node == NULL? "nodo vacio": "nodo encontrado");

    if ( existing_node != NULL ) {
        printf("[ERROR] Ya existia un nodo con ese nombre, banana!!!\n");

        return 1;
    }

    return 0;
}


void addComponent(ComponentDefRec * component, ComponentType * component_type) {
    component_t new_component;
    new_component.component_type = component_type;
    new_component.component_name = (char*)malloc(strlen(component->component_name) + 1);
    strcpy(new_component.component_name, component->component_name);
    new_component.constant = component->constant;

    if (!objectExists(new_component.component_name)) {
        // printf("%s", new_component.component_name);
        // printComponent(&new_component);

        printf("[ERROR] objectExists devolvio 0\n");
        
        addElement(symbolTable->components, &new_component);
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
    new_node.name = (char*)malloc(strlen(node->name) + 1);
    strcpy(new_node.name, node->name);

    // If existing_node is NULL, the node does not exist
    if (!objectExists(new_node.name)) {
        addElement(symbolTable->nodes, &new_node);
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


void symbolTableFree() {
    if (symbolTable == NULL) {
        printf("Symbol table already freed\n");
        return;
    }

    // Free the components and nodes lists
    freeList(symbolTable->components);
    freeList(symbolTable->nodes);

    // Free the symbol table structure
    free(symbolTable);
    symbolTable = NULL;
}
