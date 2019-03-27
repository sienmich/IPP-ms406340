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

static void extend(Vector *vector) {
	vector->maxSize = vector->maxSize * 2;
	if (vector->maxSize == 0)
		vector->maxSize = 1;

	if (!(vector->data = realloc(vector->data, vector->maxSize * sizeof(*void))))
            exit(1);
}

void* pushBack(void *ptr) {
	if (vector->size == vector->maxSize)
		extend(vector);
	return vector->data[vector->size++] = ptr;
}

/// trzeba samemu usuwać rzeczy, na które były wskaźniki
void deleteVector(Vector *vector) {
    free(vector->data);
    free(vector);
}


#endif /* __VECTOR_H__ */
