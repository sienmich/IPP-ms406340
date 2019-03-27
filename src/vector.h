#ifndef __VECTOR_H__
#define __VECTOR_H__

typedef struct Vector Vector;

Vector* newVector();

void deleteVector(Vector *vector);

void pushBack(Vector *vector, void *ptr);

#endif /* __VECTOR_H__ */
