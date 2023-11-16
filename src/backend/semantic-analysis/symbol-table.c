#include "symbol-table.h"


symbol_t *symbolTable = NULL;

List * errorList = NULL;

List * unlinkedObjects = NULL;

void addError(char * errorMessage);

symbol_t * symbolTableInit(){
    if(symbolTable != NULL){
        printf("Symbol table already initialized\n");
        return symbolTable;
    }

    symbol_t* symbol = (symbol_t*)malloc(sizeof(symbol_t));
    if (symbol == NULL) {
        return NULL; // Memory allocation failed
    }
    symbol->components = initList();
    symbol->nodes = initList();

    errorList = initList();
    unlinkedObjects = initList();

    symbolTable = symbol;
    return symbolTable;
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

// Function to print an element of type struct component_t
void printComponent(const void* data) {
    const struct component_t* component = (const struct component_t*)data;
    printf("Component Name: %s, Constant: %d\n", component->component_name, component->constant);

    printf("This dir: %p\n", component);

    if (component->prev != NULL) {
        if (component->prev_type == COMPONENT_TYPE) {
            component_t *fromComponent = (component_t *)component->prev;
            
            printf("Prev comp name: %p\n", fromComponent->component_name);

        } else if (component->prev_type == NODE_TYPE) {
            node_t *fromNode = (node_t *)component->prev;

            printf("Prev node name dir: %p\n", fromNode->name);
        }
    }
        
    printf("Prev dir: %p\n", component->prev);
    printf("Next dir: %p\n", component->next);
}

// Function to print an element of type struct component_t
void printNode(const void* data) {
    const struct node_t* node = (const struct node_t*)data;
    printf("Node Name: %s\n", node->name);
    printf("Node Dir: %p\n", node);

    printf("Dir1: %p\n", node->dir[0]);
    printf("Dir2: %p\n", node->dir[1]);
    printf("Dir3: %p\n", node->dir[2]);
    printf("Dir4: %p\n", node->dir[3]);

}

typedef struct {
    void* object; // Pointer to the found object (component or node)
    object_type type; // Type of the found object
} ObjectResult;

void * findObject(char * name, object_type * type) {
    component_t * component_search = (component_t *)malloc(sizeof(component_t)); // Create a search object
    component_search->component_name = (char*)malloc(strlen(name) + 1);
    strcpy(component_search->component_name, name);

    // firstMatch to check if the component already exists
    component_t* existing_component = (component_t*)findElement(symbolTable->components, component_search, compareComponentByName);

    if ( existing_component != NULL ) {
        *type = COMPONENT_TYPE;
        return (void *)existing_component;
    }
    
    node_t * node_search = (node_t *)malloc(sizeof(node_t)); // Create a search object
    node_search->name = (char*)malloc(strlen(name) + 1);
    strcpy(node_search->name, name);

    // firstMatch to check if the node already exists
    node_t* existing_node = (node_t*)findElement(symbolTable->nodes, node_search, compareNodeByName);

    if ( existing_node != NULL ) {
        *type = NODE_TYPE;
        return (void *) existing_node;
    }

    return NULL;
}


void addComponent(ComponentDefRec * component, ComponentType * component_type) {
    component_t * new_component = (component_t *)malloc(sizeof(component_t));
    new_component->component_type = component_type;
    new_component->component_name = (char*)malloc(strlen(component->component_name) + 1);
    strcpy(new_component->component_name, component->component_name);
    new_component->constant = component->constant;

    object_type type;

    if (findObject(new_component->component_name, &type) == NULL) {        
        addElement(symbolTable->components, new_component);
    } else {
        addError("Duplicate name declaration");
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
    node_t * new_node = (node_t *)malloc(sizeof(node_t));
    new_node->name = (char*)malloc(strlen(node->name) + 1);
    strcpy(new_node->name, node->name);

    object_type type;

    if (findObject(new_node->name, &type) == NULL) {
        addElement(symbolTable->nodes, new_node);
    } else {
        addError("Duplicate name declaration");
    }
}

void addNodes(DeclareNode * node) {
    DeclareNode * current_node = node;
    while (current_node != NULL) {
        addNode(current_node);
        current_node = current_node->declare_node;
    }
}

void placeInNextDir(node_t * current, void * object, object_type type) {

    for (int i = 0; i < 4; i++) {
        if (current->dir[i] == NULL) {
            current->dir_type[i] = type;
            current->dir[i] = object;
            return ;
        }
    }

    addError("No node connections available. MAX: 4");
}

void concatTo(char * fromObjectName, char * toObjectName) {

    object_type fromType;
    void * fromObject = findObject(fromObjectName, &fromType);

    object_type toType;
    void * toObject = findObject(toObjectName, &toType);

    if (fromObject == NULL || toObject == NULL) {
        addError("Concat to: object not found");
        return ;
    }

    if (fromObject == toObject) {
        addError("Concat: cannot concat objects to themselves");
        return ;        
    }

    if (fromType == COMPONENT_TYPE && toType == COMPONENT_TYPE) {
        component_t *fromComponent = (component_t *)fromObject;
        fromComponent->next_type = COMPONENT_TYPE;
        fromComponent->next = toObject;
        
        component_t *toComponent = (component_t *)toObject;
        toComponent->prev_type = COMPONENT_TYPE;
        toComponent->prev = fromObject;
    } else if (fromType == COMPONENT_TYPE && toType == NODE_TYPE) {
        component_t *fromComponent = (component_t *)fromObject;
        fromComponent->next_type = NODE_TYPE;
        fromComponent->next = toObject;

        node_t *toNode = (node_t *)toObject;
        placeInNextDir(toNode, fromObject, fromType);
    } else if (fromType == NODE_TYPE && toType == COMPONENT_TYPE) {
        node_t *fromNode = (node_t *)fromObject;
        placeInNextDir(fromNode, toObject, toType);

        component_t *toComponent = (component_t *)toObject;
        toComponent->prev_type = NODE_TYPE;
        toComponent->prev = fromObject;
    } else if (fromType == NODE_TYPE && toType == NODE_TYPE) {
        node_t *fromNode = (node_t *)fromObject;
        placeInNextDir(fromNode, toObject, toType);
        
        node_t *toNode = (node_t *)toObject;
        placeInNextDir(toNode, fromObject, fromType);
    } else {
        addError("Concat: type not found");
        return ; 
    }

}

void concatNodes(char * fromNodeName, char * toNodeName){

    object_type fromType;
    void * fromObject = findObject(fromNodeName, &fromType);

    object_type toType;
    void * toObject = findObject(toNodeName, &toType);

    if (fromObject == NULL || toObject == NULL) {
        addError("Concat >: object not found");
        return ;
    }

    if (fromObject == toObject) {
        addError("Concat: cannot concat objects to themselves");
        return ;        
    }

    if (fromType != NODE_TYPE || toType != NODE_TYPE) {
        addError("Concat +: only nodes can be used in + operation");
        return ;
    }

    node_t *fromNode = (node_t *)fromObject;
    placeInNextDir(fromNode, toObject, toType);
        
    node_t *toNode = (node_t *)toObject;
    placeInNextDir(toNode, fromObject, fromType);
    
}

void addError(char * errorMessage) {
    error_t * new_error = (error_t *)malloc(sizeof(error_t));
    new_error->error_message = (char*)malloc(strlen(errorMessage) + 1);
    strcpy(new_error->error_message, errorMessage);

    addElement(errorList, new_error);
}

void printError(const void * data) {
    const struct error_t* error = (const struct error_t*)data;

    printf("[ERROR] %s\n", error->error_message);
}

void printErrors() {
    Node * current = errorList->head;

    if (current != NULL) {
        printf("Errors: \n");

        while (current != NULL) {
            printError(current->data);
            current = current->next;
        }

        printf("---\n");
    }
}

int hasErrors() {
    if (errorList->head == NULL) {
        return 0;
    }

    return 1;
}

void addUnlinked(char * unlinkedName) {
    error_t * new_unlinked = (error_t *)malloc(sizeof(error_t));
    new_unlinked->error_message = (char*)malloc(strlen(unlinkedName) + 1);
    strcpy(new_unlinked->error_message, unlinkedName);

    addElement(unlinkedObjects, new_unlinked);
}

int checkUnlinked() {
    int unlinkedCount = 0;

    Node * current = symbolTable->components->head;

    while (current != NULL) {
        component_t * current_component = (component_t *)current->data;
        if (current_component->prev == NULL && current_component->next == NULL) {
            unlinkedCount++;
            addUnlinked(current_component->component_name);
        }
        current = current->next;
    }

    current = symbolTable->nodes->head;

    while (current != NULL) {
        node_t * current_node = (node_t *)current->data;
        if (current_node->dir[0] == NULL && current_node->dir[1] == NULL && current_node->dir[2] == NULL && current_node->dir[3] == NULL) {
            unlinkedCount++;
            addUnlinked(current_node->name);
        }
        current = current->next;
    }

    return unlinkedCount;
}

void printUnlinked() {
    Node * current = unlinkedObjects->head;

    if (current != NULL) {
        printf("Unlinked objects: \n");

        while (current != NULL) {
            printError(current->data);
            current = current->next;
        }

        printf("---\n");
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

    freeList(errorList);
    freeList(unlinkedObjects);

    // Free the symbol table structure
    free(symbolTable);
    symbolTable = NULL;
}