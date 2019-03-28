#include "heap.h"
#include "vector.h"

/// Na górze jest minimum
typedef struct Heap {
    Vector *v;
    bool (*cmp)(void *, void *); /// a >= b
} Heap;

Heap* newHeap( bool (*cmp)(void *, void *) ) {
    Heap *heap;
    if (!(heap = malloc(sizeof(Heap))))
        exit(1);
    heap->v = newVector();
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


void insert(Heap *heap, void *ptr) {
    pushBack(heap, ptr);
    int pos = heap->size - 1;
    while (pos != 0 && heap->cmp(heap->data[parent(pos)], heap->data[pos])) {

        swapElements(heap->v, pos, parent(pos));
        pos = parent(pos);
    }

}

void* pop(Heap *heap) {
    swapElements(heap->v, 0, heap->v->size - 1);
    void *res = popBack(heap->v);

    int pos = 0, left = 1, right = 2;
    while (left < heap->v->size) {

        if (left == heap->v->size - 1 || heap->cmp(heap->v->data[right], heap->v->data[left]) ) {//prawy juz jest poza
            if(heap->cmp(heap->data[pos], heap->data[left])) {
                swapElements(heap->v, left, pos);
                pos = left;
            }

        }
        else {
            if(heap->cmp(heap->data[pos], heap->data[right])) {
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

