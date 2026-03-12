#ifndef LINKEDLISTS_H
#define LINKEDLISTS_H
/*Linked list for the undo feature*/
typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    int size;
} LinkedList;

LinkedList* linkedList(void);
void push(LinkedList* list, void *data);
void* pop(LinkedList* list);
void freeLL(LinkedList* list, void(*freeData)(void*));

#endif
