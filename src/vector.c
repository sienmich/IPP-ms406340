#include <stdlib.h>
#include "vector.h"

Vector* newVector() {
    Vector *ptr;
    if (!(ptr = malloc(sizeof(Vector))))
        return NULL;

    ptr->maxSize = 0;
    ptr->size = 0;
    ptr->data = NULL;

    return ptr;
}

bool resize(Vector *vector, int newSize) {
	void **tmp = realloc(vector->data, newSize * sizeof(void *));
	if(tmp == NULL)
        return false;

	vector->maxSize = newSize;
    vector->data = tmp;
	return true;
}

static bool extend(Vector *vector) {
	if (vector->maxSize == 0)
        return resize(vector, 1);
	else
        return resize(vector, vector->maxSize * 2);
}

bool pushBack(Vector *vector, void *ptr) {
	if (vector->size == vector->maxSize)
        if(!extend(vector))
            return false;
	vector->data[vector->size++] = ptr;
    return true;
}

void* popBack(Vector *vector) {
	if (vector->size > 0)
        vector->size--;
    return vector->data[vector->size];
}

/// trzeba samemu usuwać rzeczy, na które były wskaźniki
void deleteVector(Vector *vector) {
    free(vector->data);
    free(vector);
}

void swapElements(Vector *vector, int a, int b) {
    void *tmp = vector->data[a];
    vector->data[a] = vector->data[b];
    vector->data[b] = tmp;
}

void deleteElementFromVectorBySwap(Vector *vector, void *ptr) {
    for (int i = 0; i < vector->size; i++) {
        if (vector->data[i] == ptr) {
            swapElements(vector, i, vector->size - 1);
            popBack(vector);
            return;
        }
    }
}
