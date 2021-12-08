#include <stdlib.h>
#include <stdio.h>
#include "list.h"

void list_init(List* list, FreeFunc freeFunc) {
	list->length = 0;
	list->freeFunc = freeFunc;
	list->head = NULL;
}

Node* list_pushBack(List* list, void* data) {
	Node* newNode = malloc(sizeof(Node));

	if (newNode == NULL) {
		return NULL;
	}

	newNode->data = data;
	newNode->next = NULL;

	if (list->head == NULL) {
		list->head = newNode;
		list->length++;
		return newNode;
	}

	Node* currentNode = list->head;

	while (currentNode->next != NULL) {
		currentNode = currentNode->next;
	}

	currentNode->next = newNode;
	list->length++;
	return newNode;
}

Node* list_pushFront(List* list, void* data) {
	Node* newNode = malloc(sizeof(Node));

	if (newNode == NULL) {
		return NULL;
	}

	newNode->data = data;
	newNode->next = list->head;

	list->head = newNode;
	list->length++;
	return newNode;
}

void freeNode(Node* node, FreeFunc freeFunc) {
	freeFunc(node->data);
	free(node);
}

List* list_remove(List* list, void* targetData) {
	if (list->head == NULL) {
		return NULL;
	}

	if (list->head->data == targetData) {
		Node* nextNode = list->head->next;
		freeNode(list->head, list->freeFunc);
 		list->head = nextNode;
		list->length--;
		return list;
	}

	Node* currentNode = list->head;

	while (currentNode->next->data != targetData) {
		if (currentNode->next == NULL) {
			return NULL;
		}
		currentNode = currentNode->next;
	}

	Node* nextNode = currentNode->next->next;
	freeNode(currentNode->next, list->freeFunc);
	currentNode->next = nextNode;
	list->length--;
	return list;
}

void list_forEach(List* list, void (*func)(void*, void*), void* arg) {
	Node* currentNode = list->head;

	while (currentNode != NULL) {
		Node* nextNode = currentNode->next;
		func(currentNode->data, arg);

		currentNode = nextNode;
	}
}