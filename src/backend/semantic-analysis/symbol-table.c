#include "symbol-table.h"


symbol_t *symbolTable = NULL;

List * errorList = NULL;

List * unlinkedObjects = NULL;

void addError(char * errorMessage, char * object_name);

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
    printf("Component Name: %s, Constant: %.3f\n", component->component_name, component->constant);

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

void setShowLabel(char * name, Boolean showName) {
    object_type type;
    void * object = findObject(name, &type);

    if (object == NULL) {
        addError("ShowLabel: object not found", name);
        return ;
    }

    if (type == COMPONENT_TYPE) {
        component_t * component = (component_t *)object;
        component->showName = showName;
    } else if (type == NODE_TYPE) {
        return;
    } else {
        addError("ShowLabel: type not found", name);
        return ;
    }
}

void addParam(const ComponentDefRec * component, const Parameter * parameter) {
    // there is only one parameter: showName. If there are more, then bison caught the error earlier
    setShowLabel(component->component_name, *parameter->boolean);
}

void addParams(ComponentDefRec * component_def_rec, ComaParameter * coma_parameter) {
    object_type type;
    char * name = malloc(strlen(component_def_rec->component_name) + 1);
    strcpy(name, component_def_rec->component_name);
    component_t * component = (component_t *)findObject(name, &type);
    if(component == NULL) {
        addError("Component not found", component->component_name);
        return ;
    }
    // if there were more parameters, then bison caught the error earlier. Currently there is only one parameter: showName
    // if we add more parameters, then we need to iterate through the coma_parameter list
    ComponentDefRec * current_component = component_def_rec;
    while (current_component != NULL) {
        addParam(current_component, coma_parameter->parameter);
        current_component = current_component->component_def_rec;
    }
}

void addComponent(ComponentDefRec * component, ComponentType * component_type) {
    component_t * new_component = (component_t *)malloc(sizeof(component_t));
    new_component->component_type = component_type;
    new_component->component_name = (char*)malloc(strlen(component->component_name) + 1);
    strcpy(new_component->component_name, component->component_name);
    new_component->constant = component->constant;
    new_component->prev = NULL;
    new_component->next = NULL;
    new_component->showName = 1;

    object_type type;

    if (findObject(new_component->component_name, &type) == NULL) {        
        addElement(symbolTable->components, new_component);
    } else {
        addError("Duplicate name declaration", new_component->component_name);
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
    for(int i = 0; i < 4; i++ ) {
        new_node->dir[i] = NULL;
    }

    object_type type;

    if (findObject(new_node->name, &type) == NULL) {
        addElement(symbolTable->nodes, new_node);
    } else {
        addError("Duplicate name declaration", new_node->name);
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

    addError("No node connections availabel. MAX: 4", current->name);
}

void concatTo(char * fromObjectName, char * toObjectName) {

    object_type fromType;
    void * fromObject = findObject(fromObjectName, &fromType);

    object_type toType;
    void * toObject = findObject(toObjectName, &toType);

    if (fromObject == NULL || toObject == NULL) {
        addError("Concat to: object not found", toObjectName);
        return ;
    }

    if (fromObject == toObject) {
        addError("Concat: cannot concat objects to themselves", fromObjectName);
        return ;        
    }

    if (fromType == COMPONENT_TYPE && toType == COMPONENT_TYPE) {
        component_t *fromComponent = (component_t *)fromObject;
        if (fromComponent->next != NULL) addError("Component concated 'to' more than once", fromComponent->component_name);
        fromComponent->next_type = COMPONENT_TYPE;
        fromComponent->next = toObject;
        
        component_t *toComponent = (component_t *)toObject;
        if (toComponent->prev != NULL) addError("Component concated 'from' more than once", toComponent->component_name);
        toComponent->prev_type = COMPONENT_TYPE;
        toComponent->prev = fromObject;
    } else if (fromType == COMPONENT_TYPE && toType == NODE_TYPE) {
        component_t *fromComponent = (component_t *)fromObject;
        if (fromComponent->next != NULL) addError("Component concated 'to' more than once", fromComponent->component_name);
        fromComponent->next_type = NODE_TYPE;
        fromComponent->next = toObject;

        node_t *toNode = (node_t *)toObject;
        placeInNextDir(toNode, fromObject, fromType);
    } else if (fromType == NODE_TYPE && toType == COMPONENT_TYPE) {
        node_t *fromNode = (node_t *)fromObject;
        placeInNextDir(fromNode, toObject, toType);

        component_t *toComponent = (component_t *)toObject;
        if (toComponent->prev != NULL) addError("Component concated 'from' more than once", toComponent->component_name);
        toComponent->prev_type = NODE_TYPE;
        toComponent->prev = fromObject;
    } else if (fromType == NODE_TYPE && toType == NODE_TYPE) {
        node_t *fromNode = (node_t *)fromObject;
        placeInNextDir(fromNode, toObject, toType);
        
        node_t *toNode = (node_t *)toObject;
        placeInNextDir(toNode, fromObject, fromType);
    } else {
        addError("Concat: type not found", fromObjectName);
        return ; 
    }

}

void concatNodes(char * fromNodeName, char * toNodeName){

    object_type fromType;
    void * fromObject = findObject(fromNodeName, &fromType);

    object_type toType;
    void * toObject = findObject(toNodeName, &toType);

    if (fromObject == NULL) {
        addError("Concat >: object not found", fromNodeName);
        return ;
    }

    if (toObject == NULL) {
        addError("Concat >: object not found", toNodeName);
        return ;
    }

    if (fromObject == toObject) {
        addError("Concat: cannot concat objects to themselves", fromNodeName);
        return ;        
    }

    if (fromType != NODE_TYPE) {
        addError("Concat +: only nodes can be used in + operation", fromNodeName);
        return ;
    }
    
    if (toType != NODE_TYPE) {
        addError("Concat +: only nodes can be used in + operation", toNodeName);
        return ;
    }

    node_t *fromNode = (node_t *)fromObject;
    placeInNextDir(fromNode, toObject, toType);
        
    node_t *toNode = (node_t *)toObject;
    placeInNextDir(toNode, fromObject, fromType);
    
}

void assignValuesToNames(ComponentDefRec * component_def_names, ComponentDefRec * component_def_values) {
    ComponentDefRec * current_name = component_def_names;
    ComponentDefRec * current_value = component_def_values;
    
    while (current_name != NULL && current_value != NULL) {
        current_name->constant = current_value->constant;

        current_name = current_name->component_def_rec;
        current_value = current_value->component_def_rec;
    }

    if (current_name != NULL || current_value != NULL) {
        addError("Mismatching component name - value", component_def_names->component_name);
    }
}

void addError(char * errorMessage, char * object_name) {
    error_t * new_error = (error_t *)malloc(sizeof(error_t));
    new_error->error_message = (char*)malloc(strlen(errorMessage) + 1);
    strcpy(new_error->error_message, errorMessage);

    new_error->error_object_name = object_name;

    addElement(errorList, new_error);
}

void printError(const void * data) {
    const struct error_t* error = (const struct error_t*)data;

    printf("[ERROR] %s: %s\n", error->error_object_name, error->error_message);
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

void addUnlinked(char * unlinkedMessage, char * unlinkedName) {
    error_t * new_unlinked = (error_t *)malloc(sizeof(error_t));
    new_unlinked->error_message = (char*)malloc(strlen(unlinkedMessage) + 1);
    strcpy(new_unlinked->error_message, unlinkedMessage);

    new_unlinked->error_object_name = unlinkedName;

    addElement(unlinkedObjects, new_unlinked);
}

int checkUnlinked() {
    int unlinkedCount = 0;

    Node * current = symbolTable->components->head;

    while (current != NULL) {
        component_t * current_component = (component_t *)current->data;
        if (current_component->prev == NULL || current_component->next == NULL) {
            unlinkedCount++;
            addUnlinked("Missing link in one or more directions", current_component->component_name);
        }
        current = current->next;
    }

    current = symbolTable->nodes->head;

    while (current != NULL) {
        int linked_count = 0;
        node_t * current_node = (node_t *)current->data;
        for(int i = 0; i < 4; i++ ) {
            if (current_node->dir[i] != NULL) linked_count++;
        }

        if(linked_count < 2) {
            unlinkedCount++;
            addUnlinked("Node linked to less than two objects", current_node->name);
        }

        current = current->next;
    }

    return unlinkedCount;
}

void printUnlinked() {
    Node * current = unlinkedObjects->head;

    if (current != NULL) {
        printf("Unlinked objects found: \n");

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