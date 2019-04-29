/** @file
 * Implementacja interfejsu klasy wektor - dynamicznie alokowanej tablicy
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 28.04.2019
 */

#include <stdlib.h>
#include "vector.h"

/** @brief Tworzy nowy wektor.
 * Tworzy nowy wektor, który nie zawiera żadnych elementów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Vector* newVector() {
    Vector *vector;
    if (!(vector = malloc(sizeof(Vector))))
        return NULL;

    vector->maxSize = 0;
    vector->size = 0;
    vector->data = NULL;

    return vector;
}
/** Alokuje nową pamięć dla wektora.
 * Nie zmienia jego rozmiaru.
 *
 * @param[in,out] vector - wskaźnik na wektor
 * @param[in] newMaxSize - nowy rozmiar alokowanej pamięci
 * @return @p false gdy nie udało się zaalkować pamięci, @p true w przeciwnym przypadku
 */
static bool reMaxSize(Vector *vector, int newMaxSize) {
    void **tmp = realloc(vector->data, newMaxSize * sizeof(void *));
    if (tmp == NULL)
        return false;

    vector->maxSize = newMaxSize;
    vector->data = tmp;
    return true;
}

/** @brief Tworzy nowy wektor o podanej długości.
 * @param [in] size - długość nowego wektora.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Vector* newVectorWithSize(int size) {
    Vector *vector = newVector();
    if (!vector)
        return NULL;

    if (!reMaxSize(vector, size)) {
        deleteVector(vector);
        return NULL;
    }

    vector->size = size;

    return vector;
}

/** @brief Usuwa wektor.
 * Usuwa wektor wskazywany przez @p vector.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param [in, out] vector - wskaźnik na wektor, który usuwa
 */
void deleteVector(Vector *vector) {
    if (!vector)
        return;
    free(vector->data);
    free(vector);
}

/** Alokuje dwa razy więcej pamięci na wektor.
 * @param vector - wskaźnik na wektor
 * @return @p false gdy nie udało się zaalkować pamięci, @p true w przeciwnym przypadku
 */
static bool extend(Vector *vector) {
	if (vector->maxSize == 0)
        return reMaxSize(vector, 1);
	else
        return reMaxSize(vector, vector->maxSize * 2);
}

/** @brief Dodaje nowy wskaźnik na koniec wektora.
 * @param[in,out] vector - wskaźnik na wektor
 * @param[in] ptr - wskaźnik, który będzie dodany na koniec
 * @return Wartość @p true, jeśli operacja się powiodła.
 * Wartość @p false, jeśli nie udało się zaalokować pamięci.
 */
bool pushBack(Vector *vector, void *ptr) {
	if (vector->size == vector->maxSize)
        if (!extend(vector))
            return false;
	vector->data[vector->size++] = ptr;
    return true;
}

/** @brief Usuwa i zwraca ostatni wskaźnik z wektora.
 * @param[in,out] vector - wskaźnik na wektor
 * @return  Ostatni wskaźnik z wektora lub NULL, gdy wektor jest pusty.
 */
void* popBack(Vector *vector) {
    if (!vector->size)
        return NULL;
    vector->size--;
    return vector->data[vector->size];
}

/** @brief Zamienia miejscami dwa elementy wektora.
 * @param[in,out] vector - wskaźnik na wektor
 * @param[in] a - indeks pierwszego elementu
 * @param[in] b - indeks drugiego elementu
 */
void swapElements(Vector *vector, int a, int b) {
    void *tmp = vector->data[a];
    vector->data[a] = vector->data[b];
    vector->data[b] = tmp;
}

/** @brief Usuwa z wektora jeden element. Może zmienić kolejność pozostałych.
 * Przegląda po kolei elementy wektora i porównuje je z [ptr].
 * Pierwszy, który będzie równy zamienia z ostatnim i usuwa.
 * Jeśli nie znajdzie równego elementu, nic nie robi.
 * @param[in,out] vector - wskaźnik na wektor
 * @param[in] ptr - wskaźnik, który usuwa z wektora
 */
void deleteElementFromVector(Vector *vector, void *ptr) {
    for (int i = 0; i < vector->size; i++) {
        if (vector->data[i] == ptr) {
            swapElements(vector, i, vector->size - 1);
            popBack(vector);
            return;
        }
    }
}