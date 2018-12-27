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
#include "bspGpio.h"

typedef struct Node {
	struct Node *next;
	Event_t evt;
} Node_t;

struct {
	Node_t *head;
    Node_t *tail;
} s_queue;

static inline Node_t *newNode(EventTypes_e type, void *data, onEvtDispose_f dispose) __attribute__((always_inline));

void EventQueue_Push(EventTypes_e type, void *data, onEvtDispose_f dispose) {
    int wkup = 0;
	System_Lock();
	if (!s_queue.head) {
		s_queue.head = s_queue.tail = newNode(type, data, dispose);
		wkup = 1;
	} else {
	    s_queue.tail->next = newNode(type, data, dispose);
	    s_queue.tail = s_queue.tail->next;
	}
	System_Unlock();
	if (wkup)
	    System_Wakeup();
}

void EventQueue_Pend(Event_t *event) {
	BSP_Gpio_SetPin(GPIO_LED_RED, 0);
	while (!s_queue.head)
		System_Poll();
	BSP_Gpio_SetPin(GPIO_LED_RED, 1);
	System_Lock();
	Node_t *node = s_queue.head;

	event ?
		*event = node->evt :
		EventQueue_Dispose(&node->evt);
	s_queue.head = node->next;
	if (!s_queue.head)
	    s_queue.tail = NULL;
	free(node);
	System_Unlock();
}

void EventQueue_Dispose(Event_t *event) {
	if (event && event->dispose) {
		event->dispose(event->data);
		event->data = NULL;
		event->dispose = NULL;
	}
}

static Node_t *newNode(EventTypes_e type, void *data, onEvtDispose_f dispose) {
	Node_t *node = malloc(sizeof(Node_t));
	if (!node)
	    return NULL;
    node->evt.type = type;
    node->evt.data = data;
    node->evt.dispose = dispose;
    node->next = NULL;
	return node;
}

