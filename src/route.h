/** @file
 * Interfejs klasy trasa przechowywującej drogi krajowe
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 28.04.2019
 */

#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "vector.h"
#include "city.h"
/** Trasa, czyli droga krajowa.
 *  Składa się z wektora miast i dróg. Miast jest o jedno więcej.
 */
typedef struct Route {
    Vector *roads; ///< Wskaźnik na wektor kolejnych dróg
    Vector *cities; ///< Wskaźnik na wektor kolejnych miast
    int idx; ///< Numer drogi krajowej
} Route;

/** Tworzy nową trasę.
 * Tworzy nową trasę, która ma pustą listę miast i dróg.
 * @param[in] idx - numer trasy
 * @return Wskaźnik na nową strukturę lub NULL, gdy nie udało się zaalokować pamięci.
 */
Route* nRoute(int idx);

/** Usuwa trasę.
 * Zwalnia pamięć zaalokowaną pod trasę.
 * Dodatkowo usuwa wskaźnik na tę trasę ze wszystkich dróg na niej występujących.
 * Nic nie robi, gdy wskaźnik jest równy NULL.
 * @param[in,out] route - wskaźnik na usuwaną strukturę
 */
void deleteRoute(Route *route);


/** Zwraca wskaźnik na napis opisujący trasę.
 * Alokuje pamięć, należy potem ją zwolnić.
 * Napis jest zgodny ze specyfikacją @ref getRouteDescription.
 * @param[in,out] route - wskaźnik na trasę
 * @return Wskaźnik na napis lub NULL, gdy nie udao się zaalokować pamięci.
 */
char* RouteToString(Route *route);

/** Znajduje optymalną trasę między dwoma miastami.
 * Optymalną, tzn. najkrótszą, zgodną ze specyfikacją @ref newRoute.
 * Znajduje ją za pomocą algorytmu Dijikstry, działa w czasie O(m log m), gdzie m to liczba dróg na całej mapie.
 *
 * @param[in] cities - wskaźnik na wektor wszystkich miast
 * @param[in] source - wskaźnik na początkowe miasto
 * @param[in] target - wskaźnik na końcowe miasto
 * @param[in] avoid - wskaźnik na drogę, którą omijamy (może być NULL)
 * @param[in] avoid2 - wskaźnik na trasę, której miasta omijamy (może być NULL)
 * @return Wskaźnik na nowo utworzoną strukturę lub NULL, gdzy nie udało się znaleźć takiej trasy lub zaalokować pamięci
 */
Route* dijikstra(Vector *cities, City *source, City *target, Road *avoid, Route *avoid2);

/** Sprawdza, czy trasa jest poprawna.
 * Sprawdza, czy trasa jest poprawna, czyli czy nie zawiera samoprzecięć.
 * @param[in] route - wskaźnik na sprawdzaną trasę
 * @return @p true jeśli jest poprawna, @p false jeśli nie jest.
 */
bool valid(Route *route);

/** Dodaje do trasy fragment innej trasy.
 * Dodaje @p end - @p begin - 1 miast oraz dróg, począwszy od miasta na pozycji @p begin.
 * @param[in,out] route
 * @param[in] add
 * @param[in] begin
 * @param[in] end
 * @return @p false gdy nie udało się zaalokować pamięci (wtedy nie modyfikuje @p route) lub @p true w przeciwnym przypadku
 */
bool addRoute(Route* route, Route* add, int begin, int end);

/** Znajduje optymalną trasę nieprzechodzącą przez podany odcinek drogi.
 * Optymalną, tzn. najkrótszą, zgodną ze specyfikacją @ref newRoute.
 * @param[in] cities - wskaźnik na wektor wszystkich miast
 * @param[in] route - wskaźnik na starą trasę
 * @param[in] road - wskaźnik na omijaną drogę
 * @return Wskaźnik na nowa trasę lub NULL, gdy nie istnieje taki objazd lub nie udało się zaalokować pamięci.
 */
Route* bypass(Vector *cities, Route* route, Road *road);

/** Liczy długość i rok trasy.
 *
 * @param[in] route - trasa
 * @return Dystans całej trasu lub NULL, gdy nie udało się zainicjalizować pamięci.
 */
Distance *getDistance(Route *route);

#endif /* __ROUTE_H__ */
