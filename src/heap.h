/** @file
 * Interfejs klasy kopiec - kolejki priorytetowej opartej na kopcu binarnym
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 28.04.2019
 */
#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdbool.h>
#include "vector.h"

/** @brief Kopiec binarny.
* Na początku ma długość 0. Kiedy dodejemy na koniec nowy element,
* ale nie mieści się on w pamięci, alokuję dwa razy większą tablicę.
* Dzięki temu złożoność czasowa i pamięciowa wektora są liniowe.
*/
typedef struct Heap Heap;

/** @brief Tworzy nowy kopiec.
 * Tworzy nowy kopiec, który nie zawiera żadnych elementów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Heap* newHeap(bool (*cmp)(void *, void *));

/** @brief Usuwa kopiec.
 * Usuwa kopiec wskazywany przez @p heap.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param [in, out] heap - wskaźnik na kopiec, który usuwa
 */
void deleteHeap(Heap *heap);

/** @brief Dodaje nowy wskaźnik do kopca.
 * @param[in,out] heap - wskaźnik na wektor
 * @param[in] ptr - wskaźnik, który będzie dodany
 * @return Wartość @p true, jeśli operacja się powiodła.
 * Wartość @p false, jeśli nie udało się zaalokować pamięci.
 */
bool insert(Heap *heap, void *ptr);

/** @brief Usuwa i zwraca minimalny element z kopca.
 * @param[in,out] heap - wskaźnik na kopiec
 * @return Minimalny wskaźnik z kopca lub NULL, gdy kopiec jest pusty.
 */
void* pop(Heap *heap);

/** @brief Sprawdza czy kopiec jest pusty.
 * @param[in,out] heap - wskaźnik na kopiec
 * @return Wartość @p true, gdy kopiec jest pusty.
 * Wartość @p false, jeśli nie jest pusty.
 */
bool empty(Heap *heap);

#endif /* __HEAP_H__ */
