/** @file
 * Interfejs klasy kopiec - kolejki priorytetowej na kopcu binarnym
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 28.04.2019
 */
#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdbool.h>
#include "vector.h"

/** Kopiec binarny.
* Każdy wierzchołek kopca jest mniejszy niż jego synowie,
* czyli minimalny wierzchołek jest w korzeniu.
* Modyfikatory działają w czasie logarytmicznym ze względu
* na aktualną wielkość kopca.
* Pamięta wszystkie wierzchołki kopca na wektorze.
* Wierzchołek 0 to korzeń, 1 to jego lewy syn, 2 to prawy syn itd.
*/
typedef struct Heap Heap;

/** Tworzy nowy kopiec.
 * Tworzy nowy kopiec, który nie zawiera żadnych elementów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Heap *newHeap(bool (*cmp)(void *, void *));

/** Usuwa kopiec.
 * Usuwa kopiec wskazywany przez @p heap.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param [in, out] heap - wskaźnik na kopiec, który usuwa
 */
void deleteHeap(Heap *heap);

/** Dodaje nowy wskaźnik do kopca.
 * @param[in,out] heap - wskaźnik na wektor
 * @param[in] ptr - wskaźnik, który będzie dodany
 * @return Wartość @p true, jeśli operacja się powiodła.
 * Wartość @p false, jeśli nie udało się zaalokować pamięci.
 */
bool insert(Heap *heap, void *ptr);

/** Usuwa i zwraca minimalny element z kopca.
 * @param[in,out] heap - wskaźnik na kopiec
 * @return Minimalny wskaźnik z kopca lub NULL, gdy kopiec jest pusty.
 */
void *pop(Heap *heap);

/** Sprawdza czy kopiec jest pusty.
 * @param[in,out] heap - wskaźnik na kopiec
 * @return Wartość @p true, gdy kopiec jest pusty.
 * Wartość @p false, jeśli nie jest pusty.
 */
bool empty(Heap *heap);

#endif /* __HEAP_H__ */
