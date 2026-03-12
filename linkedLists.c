#include <stdlib.h>
#include <stdio.h>
#include "genMap.h"
#include "gameFunc.h"
#include "linkedLists.h"

/*Constructor for linked list*/
LinkedList* linkedList(void) {
    /*Allocate the memory*/
    LinkedList* result;
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));

    /*Check if the copy isn't null/memory allocated*/
    if (list == NULL) {
        result = NULL;
    } else {
        list->head = NULL;
        list->size = 0;
        result = list;
    }

    return result;
}

/*Function to add to the linkedlist*/
void push(LinkedList* list, void *data) {
    Node* node;

    /*Check that the list has data*/
    if (list != NULL) {
        /*allocate memory*/
        node = (Node*)malloc(sizeof(Node));

        /*Check that the node has memory allocated to it*/
        if(node != NULL) {
            node->data = data;
            node->next = list->head;
            list->head = node;
            list->size++;
        }
    }
}

/*Method to remove a node from the linkedlist*/
void* pop(LinkedList* list) {
    Node* temp;
    void* data;

    /*make sure there is data in the list to pop/remove*/
    if (list == NULL || list->head == NULL) {
        data = NULL;
    } 
    else {
        /*Get the data from the head and move the head to the next node*/
        temp = list->head;
        data = temp->data;
        list->head = temp->next;
        list->size--;

        free(temp);
    }

    return data;
}

/*Method to free all the allocated memory for the linkedlist*/
void freeLL(LinkedList* list, void(*freeData)(void*)) {
    Node* currentNode;
    Node* nextNode;

    /*Check that the list has data*/
    if (list != NULL) {
        currentNode = list->head;

        /*Iterate through the list and free each node*/
        while(currentNode != NULL) {
            nextNode = currentNode->next;
            if (freeData != NULL && currentNode->data != NULL) {
                freeData(currentNode->data);
            }
            free(currentNode);
            currentNode = nextNode;
        }
        
        free(list);
    }
}

