/*
 * Queue.c
 *
 *	Created on: Apr 20, 2016
 *		Author: shapa
 */

#include "Queue.h"
#include <stdlib.h>
#include <string.h>
#include "system.h"
#include "memman.h"

typedef struct Node {
	struct Node *next;
	Event_t evt;
} Node_t;

struct {
	Node_t *head;
    Node_t *tail;
} s_queue;

static inline Node_t *newNode(EventTypes_e type, void *data, onEvtDispose_f dispose);

void EventQueue_Push(EventTypes_e type, void *data, onEvtDispose_f dispose) {
	int primask = System_Lock();
	if (!s_queue.head) {
		s_queue.head = s_queue.tail = newNode(type, data, dispose);
	} else {
	    s_queue.tail->next = newNode(type, data, dispose);
	}
	System_Unlock(primask);
}

void EventQueue_Pend(Event_t *event) {
	while (!s_queue.head)
		System_Poll();
	int primask = System_Lock();
	Node_t *node = s_queue.head;

	event ?
		*event = node->evt :
		EventQueue_Dispose(&node->evt);
	s_queue.head = node->next;
	if (!s_queue.head)
	    s_queue.tail = NULL;
	MEMMAN_free(node);
	System_Unlock(primask);
}

void EventQueue_Dispose(Event_t *event) {
	int primask = System_Lock();
	if (event && event->dispose) {
		event->dispose(event->data);
		event->data = NULL;
		event->dispose = NULL;
	}
	System_Unlock(primask);
}

static Node_t *newNode(EventTypes_e type, void *data, onEvtDispose_f dispose) {
	Node_t *node = MEMMAN_malloc(sizeof(Node_t));
	if (!node)
	    return NULL;
    node->evt.type = type;
    node->evt.data = data;
    node->evt.dispose = dispose;
    node->next = NULL;
	return node;
}

