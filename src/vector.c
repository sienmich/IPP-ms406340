#include "vector.h"

typedef struct Vector {
    int maxSize;
    int size;
	void **data;
} Vector;

Vector* newVector() {
    Vector *ptr;
    if (!(ptr = malloc(sizeof(Vector))))
        exit(1);

    ptr->maxSize = 0;
    ptr->size = 0;

    return ptr;
}

void resize(Vector *vector, int newSize) {
	vector->maxSize = newSize;

	if (!(vector->data = realloc(vector->data, vector->maxSize * sizeof(*void))))
            exit(1);
}

static void extend(Vector *vector) {
	if (vector->maxSize == 0)
        resize(vector, 1);
	else
        resize(vector, vector->maxSize * 2);
}

void* pushBack(Vector *vector, void *ptr) {
	if (vector->size == vector->maxSize)
		extend(vector);
	return vector->data[vector->size++] = ptr;
}


void* popBack(Vector *vector) {
	if (vector->size > 0)
        vector->size--;
    return vector->data->size;
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
