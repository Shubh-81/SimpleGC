#include <stdio.h>
#include <stdlib.h>

#define STACK_MAX 256
#define INITIAL_GC_THRESHOLD 8

typedef enum {
    OBJ_INT,
    OBJ_PAIR,
} ObjectType;

typedef struct sObject {
    unsigned char marked;
    ObjectType type;
    struct sObject* next;

    union {
        int value;
        struct {
            struct sObject* head;
            struct sObject* tail;
        };
    };
} Object;

typedef struct {
    int numObjects;
    int maxObjects;
    Object* stack[STACK_MAX];
    Object* firstObject;
    int stackSize;
} VM;

void gc(VM* vm);

void assert(int condition, const char* message) {
    if(!condition) {
        printf("%s\n",message);
        exit(1);
    }
}

VM* newVM() {
    VM* vm = malloc(sizeof(VM));
    vm->stackSize = 0;
    vm->firstObject = NULL;
    vm->numObjects = 0;
    vm->maxObjects = INITIAL_GC_THRESHOLD;
    return vm;
}

void push(VM* vm, Object* value) {
    assert(vm->stackSize < STACK_MAX, "Stack Overflow!");
    vm->stack[vm->stackSize++] = value;
}

Object* pop(VM* vm) {
    assert(vm->stackSize > 0, "Stack Underflow!");
    return vm->stack[--vm->stackSize];
}

Object* newObject(VM* vm, ObjectType type) {
    if(vm->numObjects == vm->maxObjects)    gc(vm);

    Object* object = malloc(sizeof(Object));
    object->type = type;
    object->marked = 0;

    object->next = vm->firstObject;
    vm->firstObject = object;
    vm->numObjects++;
    return object;
}

void pushInt(VM* vm, int val) {
    Object* object = newObject(vm, OBJ_INT);
    object->value = val;
    push(vm, object);
}

Object* pushPair(VM* vm) {
    Object* object = newObject(vm, OBJ_PAIR);
    object->tail = pop(vm);
    object->head = pop(vm);

    push(vm, object);
    return object;
}

void mark(Object* object) {
    if(object->marked) return;

    object->marked = 1;

    if(object->type == OBJ_PAIR) {
        mark(object->tail);
        mark(object->head);
    }
}

void markAll(VM* vm) {
    for(int i=0; i<vm->stackSize; i++) {
        mark(vm->stack[i]);
    }
}

void sweep(VM* vm) {
    Object** object = &vm->firstObject;
    while(*object) {
        if(!(*object)->marked) {
            Object* unreached = *object;
            *object = unreached->next;
            free(unreached);
            vm->numObjects--;
        } else {
            (*object)->marked = 0;
            object = &(*object)->next;
        }
    }
}

void gc(VM* vm) {
    int numObjects = vm->numObjects;
    markAll(vm);
    sweep(vm);
    vm->maxObjects = vm->numObjects == 0 ? INITIAL_GC_THRESHOLD : vm->numObjects * 2;
    printf("Collected %d objects, %d remaining.\n", numObjects - vm->numObjects, vm->numObjects);
}

void freeVM(VM* vm) {
    vm->stackSize = 0;
    gc(vm);
    free(vm);
}







