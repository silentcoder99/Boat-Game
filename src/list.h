#ifndef LIST_H
#define LIST_H

typedef void (*FreeFunc)(void*);

typedef struct Node {
	void* data;
	struct Node* next;
} Node;

typedef struct {
	int length;
	FreeFunc freeFunc;
	Node* head;
} List;

void list_init(List* list, FreeFunc freeFunc);
Node* list_pushBack(List* list, void* data);
Node* list_pushFront(List* list, void* data);
List* list_remove(List* list, void* targetNode);
void list_forEach(List* list, void (*func)(void*, void*), void* arg);

#endif