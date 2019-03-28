#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdbool.h>

typedef struct Heap Heap;

Heap* newHeap(bool (*cmp)(void *, void *));

void deleteHeap(Heap *heap);

void insert(Heap *heap, void *ptr);

void* pop(Heap *heap);

bool empty(Heap *heap);

#endif /* __HEAP_H__ */
