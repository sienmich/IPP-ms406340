#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct Vector Vector;

Vector* newVector();

void deleteVector(Vector *vector);

void resize(Vector *vector, int newSize);

void* pushBack(Vector *vector, void *ptr);

void* popBack(Vector *vector);

void swapElements(Vector *vector, int a, int b);

#endif /* __VECTOR_H__ */
