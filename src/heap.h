#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdbool.h>
#include "vector.h"

/// Na górze jest minimum
typedef struct Heap {
    Vector *v;
    bool (*cmp)(void *, void *); /// a >= b
} Heap;

Heap* newHeap(bool (*cmp)(void *, void *));

void deleteHeap(Heap *heap);

bool insert(Heap *heap, void *ptr);

void* pop(Heap *heap);

bool empty(Heap *heap);

#endif /* __HEAP_H__ */
