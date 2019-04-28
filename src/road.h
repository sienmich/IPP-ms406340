/** @file
 * Interfejs klasy droga
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 28.04.2019
 */
#ifndef __ROAD_H__
#define __ROAD_H__

#include <stdbool.h>

typedef struct Road Road;

#include "city.h"
#include "vector.h"
#include "map.h"
#include "route.h"

/** Droga.
 * Droga to krawędź grafu, która łączy ze sobą dwa miasta.
 * Kolejność tych dwóch miast jest nieistotna.
 */
typedef struct Road {
    City *city1; ///< Wskaźniki na pierwsze miasto
    City *city2; ///< Wskaźniki na drugie miasto
    unsigned length; ///< Długość drogi
    int builtYear; ///< Rok budowy lub ostatniego remontu
    Vector *routes; ///< Wskaźnik na wektor tras, na których leży
} Road;

/** Tworzy nową drogę.
 * Tworzy nową drogę łązczącą dwa miasta.
 * @param[in] city1 - wskaźnik na pierwsze miasto
 * @param[in] city2 - wskaźnik na drugie miasto
 * @param[in] length - długość drogi
 * @param[in] builtYear - rok budowy drogi
 * @return Wskaźnik na nowo utworzoną strukturę lub NULL, jeśli nie udało się zaalokować pamięci.
 */
Road* newRoad(City *city1, City *city2, unsigned length, int builtYear);

/** Zwalnia pamięć zadeklarowaną jako droga i usuwa ją z list sąsiedzctwa łączonych miast.
 * Nie modyfikuje tras, na których znajduje się dana droga.
 * Zatem można użyć tej funkcji tylko wtedy, gdy droga na pewno nie należy do żadnej trasy.
 * @param[in,out] road - wskaźnik na drogę
 */
void deleteRoadUnsafe(Road *road);

/** Usuwa drogę.
 * Usuwa drogę. Naprawia za pomocą objazdów (zgodnie ze specyfikacją @ref removeRoad) wszystkie trasy, które korzystały z tej drogi.
 *
 * @param[in,out] road - wskaźnik na usuwaną drogę
 * @param[in] cities - wskaźnik na wektor zawierający wszystkie miasta
 * @param[in,out] routes - wskaźnik na wektor zawierający wszystkie trasy
 * @return @p false gdy wystąpił błąd - nie da się uzupełnić tras lub nie udało się zaalokować pamięci.
 * Wtedy podane dane pozostają niezmodyfikowane. @p true w przeciwnym przypadku.
 */
bool deleteRoad(Road *road, Vector *cities, Vector *routes);

/** Zwraca wskaźnik na drugi koniec drogi.
 * @param[in] road - wskaźnik na drogę
 * @param[in] city - wskaźnik na miasto będące jednym końcem drogi
 * @return wskaźnik na miasto będące drugim końcem drogi
 */
City *otherCity(Road *road, City *city);

#endif /* __ROAD_H__ */
