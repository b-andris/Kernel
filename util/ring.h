/*
 * ring.h
 *
 *  Created on: 19.02.2015
 *      Author: pascal
 */

#ifndef RING_H_
#define RING_H_

#include "stddef.h"

typedef struct ring_entry_s{
	void* value;
	struct ring_entry_s* next;
	struct ring_entry_s* prev;
}ring_entry_t;

typedef struct{
	ring_entry_t* base;
	size_t size;
}ring_t;

ring_t *ring_create();
void ring_destroy(ring_t *ring);
size_t ring_size(ring_t* ring);
void* ring_add(ring_t *ring, void *value);
void* ring_remove(ring_t* ring, void* entry);
void* ring_getNext(ring_t* ring);
void* ring_find(ring_t* ring, void* value);

#endif /* RING_H_ */
