/** @file
 * Implementacja interfejsu klasy droga
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 28.04.2019
 */

#include <stdlib.h>
#include <stdbool.h>
#include "road.h"

/** Tworzy nową drogę.
 * Tworzy nową drogę łązczącą dwa miasta.
 * @param[in] city1 - wskaźnik na pierwsze miasto
 * @param[in] city2 - wskaźnik na drugie miasto
 * @param[in] length - długość drogi
 * @param[in] builtYear - rok budowy drogi
 * @return Wskaźnik na nowo utworzoną strukturę lub NULL,
 * jeśli nie udało się zaalokować pamięci.
 */
Road *newRoad(City *city1, City *city2, unsigned length, int builtYear) {
    Road *ptr;
    if (!(ptr = malloc(sizeof(Road))))
        return NULL;

    if (!(ptr->routes = newVector())) {
        free(ptr);
        return NULL;
    }

    ptr->city1 = city1;
    ptr->city2 = city2;
    ptr->length = length;
    ptr->builtYear = builtYear;

    return ptr;
}

/** Usuwa drogę.
 * Usuwa drogę. Naprawia za pomocą objazdów (zgodnie ze specyfikacją
 * @ref removeRoad) wszystkie trasy, które korzystały z tej drogi.
 *
 * @param[in,out] road - wskaźnik na usuwaną drogę
 * @param[in] cities - wskaźnik na wektor zawierający wszystkie miasta
 * @param[in,out] routes - wskaźnik na wektor zawierający wszystkie trasy
 * @return @p false gdy wystąpił błąd - nie da się uzupełnić tras lub
 * nie udało się zaalokować pamięci.
 * Wtedy podane dane pozostają niezmodyfikowane. @p true w przeciwnym przypadku.
 */
bool deleteRoad(Road *road, Vector *cities, Vector *routes) {
    Vector *altRoutes = newVector();
    if (!altRoutes)
        return false;

    for (int i = 0; i < road->routes->size; i++) {
        Route *alt = bypass(cities, road->routes->data[i], road);
        if (!alt)
            break;
        if (!pushBack(altRoutes, alt)) {
            deleteRoute(alt);
            break;
        }
    }

    if (road->routes->size != altRoutes->size) {
        for (int i = 0; i < altRoutes->size; i++)
            deleteRoute(altRoutes->data[i]);
        deleteVector(altRoutes);
        return false;
    }

    while (road->routes->size) {
        deleteRoute(road->routes->data[0]);
    }

    for (int i = 0; i < altRoutes->size; i++) {
        Route *r = altRoutes->data[i];
        routes->data[r->id] = r;
    }

    deleteVector(altRoutes);

    deleteRoadUnsafe(road);

    return true;
}

/** Zwalnia pamięć  i usuwa drogę z list sąsiedzctwa łączonych miast.
 * Nie modyfikuje tras, na których znajduje się dana droga.
 * Zatem można użyć tej funkcji tylko wtedy,
 * gdy droga na pewno nie należy do żadnej trasy.
 * @param[in,out] road - wskaźnik na drogę
 */
void deleteRoadUnsafe(Road *road) {
    deleteVector(road->routes);
    deleteRoadFromCity(road->city1, road);
    deleteRoadFromCity(road->city2, road);
    free(road);
}

/** Zwraca wskaźnik na drugi koniec drogi.
 * @param[in] road - wskaźnik na drogę
 * @param[in] city - wskaźnik na miasto będące jednym końcem drogi
 * @return wskaźnik na miasto będące drugim końcem drogi
 */
City *otherCity(Road *road, City *city) {
    if (city != road->city1)
        return road->city1;
    return road->city2;
}
