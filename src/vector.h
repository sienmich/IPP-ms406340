#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdbool.h>

typedef struct Vector {
    int maxSize;
    int size;
	void **data;
} Vector;

Vector* newVector();

void deleteVector(Vector *vector);

bool resize(Vector *vector, int newSize);

bool pushBack(Vector *vector, void *ptr);

void* popBack(Vector *vector);

void swapElements(Vector *vector, int a, int b);

void deleteElementFromVectorBySwap(Vector *vector, void *ptr);

#endif /* __VECTOR_H__ */
