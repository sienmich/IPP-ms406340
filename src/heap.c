/** @file
 * Implementacja interfejsu klasy kopiec - kolejki priorytetowej na kopcu binarnym
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 28.04.2019
 */
#include <stdlib.h>
#include "heap.h"

/** Kopiec binarny.
* Każdy wierzchołek kopca jest mniejszy niż jego synowie,
* czyli minimalny wierzchołek jest w korzeniu.
* Modyfikatory działają w czasie logarytmicznym ze względu
* na aktualną wielkość kopca.
* Pamięta wszystkie wierzchołki kopca na wektorze.
* Wierzchołek 0 to korzeń, 1 to jego lewy syn, 2 to prawy syn itd.
*/
typedef struct Heap {
    Vector *v; ///< Wskaźnik na wektor zawierający wierzchołki kopca
    bool (*cmp)(void *, void *); ///< Wskaźnik na komparator elementów kopca
} Heap;

/** Tworzy nowy kopiec.
 * Tworzy nowy kopiec, który nie zawiera żadnych elementów.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Heap *newHeap(bool (*cmp)(void *, void *)) {
    Heap *heap;
    if (!(heap = malloc(sizeof(Heap))))
        return NULL;

    if (!(heap->v = newVector())) {
        free(heap);
        return NULL;
    }
    heap->cmp = cmp;
    return heap;
}

/** Usuwa kopiec.
 * Usuwa kopiec wskazywany przez @p heap.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param [in, out] heap - wskaźnik na kopiec, który usuwa
 */
void deleteHeap(Heap *heap) {
    if (!heap)
        return;
    deleteVector(heap->v);
    free(heap);
}

/** Liczy numer ojca.
 * @param[in] x - numer wierzchołka, którego ojca szukamy
 * @return Numer ojca wierzchołka @p x.
 */
static int parent(int x) {
    return (x + 1) / 2 - 1;
}

/** Liczy numer lewego syna.
 * @param[in] x - numer wierzchołka, którego lewego syna szukamy
 * @return Numer lewego syna wierzchołka @p x.
 */
static int son(int x) {
    return (x + 1) * 2 - 1;
}

/** Dodaje nowy wskaźnik do kopca.
 * @param[in,out] heap - wskaźnik na wektor
 * @param[in] ptr - wskaźnik, który będzie dodany
 * @return Wartość @p true, jeśli operacja się powiodła.
 * Wartość @p false, jeśli nie udało się zaalokować pamięci.
 */
bool insert(Heap *heap, void *ptr) {
    if (!pushBack(heap->v, ptr))
        return false;

    int pos = heap->v->size - 1;
    while (pos && heap->cmp(heap->v->data[parent(pos)], heap->v->data[pos])) {
        swapElements(heap->v, pos, parent(pos));
        pos = parent(pos);
    }

    return true;
}

/** Sprawdza czy kopiec jest pusty.
 * @param[in,out] heap - wskaźnik na kopiec
 * @return Wartość @p true, gdy kopiec jest pusty.
 * Wartość @p false, jeśli nie jest pusty.
 */
bool empty(Heap *heap) {
    return heap->v->size == 0;
}

/** Usuwa i zwraca minimalny element z kopca.
 * @param[in,out] heap - wskaźnik na kopiec
 * @return Minimalny wskaźnik z kopca lub NULL, gdy kopiec jest pusty.
 */
void *pop(Heap *heap) {
    if (empty(heap))
        return NULL;

    swapElements(heap->v, 0, heap->v->size - 1);
    void *res = popBack(heap->v);

    int pos = 0, left = 1, right = 2;
    while (left < heap->v->size) {

        if (left == heap->v->size - 1 ||
            heap->cmp(heap->v->data[right], heap->v->data[left])) {
            if (heap->cmp(heap->v->data[pos], heap->v->data[left])) {
                swapElements(heap->v, left, pos);
                pos = left;
            }

        } else {
            if (heap->cmp(heap->v->data[pos], heap->v->data[right])) {
                swapElements(heap->v, right, pos);
                pos = right;
            }

        }
        if (pos < left)
            break;

        left = son(pos);
        right = left + 1;
    }

    return res;
}

