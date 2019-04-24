#include <stdlib.h>
#include "heap.h"

Heap* newHeap( bool (*cmp)(void *, void *) ) {
    Heap *heap;
    if (!(heap = malloc(sizeof(Heap))))
        return NULL;

    if (!(heap->v = newVector())) {
        free(heap);
        return NULL;
    }
    heap->cmp = cmp;
    return heap;
}

/// trzeba samemu usuwać rzeczy, na które były wskaźniki
void deleteHeap(Heap *heap) {
    deleteVector(heap->v);
    free(heap);
}

static int parent(int x) {
    return (x + 1) / 2 - 1;
}

static int son(int x) {
    return (x + 1) * 2 - 1;
}


bool insert(Heap *heap, void *ptr) {
    if(!pushBack(heap->v, ptr))
        return false;

    int pos = heap->v->size - 1;
    while (pos != 0 && heap->cmp(heap->v->data[parent(pos)], heap->v->data[pos])) {

        swapElements(heap->v, pos, parent(pos));
        pos = parent(pos);
    }

    return true;
}

void* pop(Heap *heap) {
    swapElements(heap->v, 0, heap->v->size - 1);
    void *res = popBack(heap->v);

    int pos = 0, left = 1, right = 2;
    while (left < heap->v->size) {

        if (left == heap->v->size - 1 || heap->cmp(heap->v->data[right], heap->v->data[left]) ) {//prawy juz jest poza
            if(heap->cmp(heap->v->data[pos], heap->v->data[left])) {
                swapElements(heap->v, left, pos);
                pos = left;
            }

        }
        else {
            if(heap->cmp(heap->v->data[pos], heap->v->data[right])) {
                swapElements(heap->v, right, pos);
                pos = right;
            }

        }
        if (pos < left)
            break;

        left = son(pos);
        right = left + 1;
    }

    return res;
}

bool empty(Heap *heap) {
    return heap->v->size == 0;
}

