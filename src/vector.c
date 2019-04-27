#include <stdlib.h>
#include "vector.h"

Vector* newVector() {
    Vector *vector;
    if (!(vector = malloc(sizeof(Vector))))
        return NULL;

    vector->maxSize = 0;
    vector->size = 0;
    vector->data = NULL;

    return vector;
}

static bool resize(Vector *vector, int newSize) {
    void **tmp = realloc(vector->data, newSize * sizeof(void *));
    if(tmp == NULL)
        return false;

    vector->maxSize = newSize;
    vector->data = tmp;
    return true;
}

Vector* newVectorWithSize(int size) {
    Vector *vector = newVector();
    if (!vector)
        return NULL;

    if (!resize(vector, size)) {
        deleteVector(vector);
        return NULL;
    }

    vector->size = size;

    return vector;
}

void deleteVector(Vector *vector) {
    if (!vector)
        return;
    free(vector->data);
    free(vector);
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
    if (!vector->size)
        return NULL;
    vector->size--;
    return vector->data[vector->size];
}


void swapElements(Vector *vector, int a, int b) {
    void *tmp = vector->data[a];
    vector->data[a] = vector->data[b];
    vector->data[b] = tmp;
}

void deleteElementFromVector(Vector *vector, void *ptr) {
    for (int i = 0; i < vector->size; i++) {
        if (vector->data[i] == ptr) {
            swapElements(vector, i, vector->size - 1);
            popBack(vector);
            return;
        }
    }
}

bool pushBackAll(Vector *vector, Vector *more) {
    for (int i = 0; i < more->size; i++) {
        if(!pushBack(vector, more->data[i]))
            return false;
    }
    return true;
}
