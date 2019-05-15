/** @file
 * Interfejs klasy miasto
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 28.04.2019
 */
#ifndef __CITY_H__
#define __CITY_H__

typedef struct City City;

#include <stdbool.h>
#include "vector.h"
#include "distance.h"
#include "road.h"

/** Miasto.
 * Przechowuje miasto jako wierzchołek grafu.
 * Pamięta jego sąsiadów w postaci listy dróg z niego wychodzących.
*/
typedef struct City {
    char *name; ///< Wskaźnik na nazwę miasta
    Vector *roads; ///< Wskaźnik na wektor dróg, które wychodzą z miasta
    Distance *distance; ///< Wskaźnik na dystans z miasta do źródła,
    ///< do algorytmu dijikstry
    Distance *distance2; ///< Wskaźnik na drugi najlepszy dystans
    ///< z miasta do źródła, do algorytmu dijikstry
} City;

/** Tworzy nowe miasto.
 * Tworzy nowe miasto o podanej nazwie, które nie posiada żadnych wychodzących dróg.
 * Parametr @p distance nie jest inicjalizowany.
 * @param name [in] - nazwa nowego miasta. Jest kopiowana.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci lub nazwa jest niepoprawna.
 */
City *newCity(const char *name);

/** Usuwa miasto.
 * Usuwa miasto wskazywane przez @p city.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param [in, out] city - wskaźnik na miasto, które usuwa
 */
void deleteCity(City *city);

/** Usuwa drogę z miasta.
 * Usuwa drogę z listy sąsiedztwa danego miasta.
 * Jeśli ta droga nie przechodzi przez to miasto, nic nie robi.
 * @param [in,out] city - wskaźnik na miasto
 * @param road - wskaźnik na drogę
 */
void deleteRoadFromCity(City *city, Road *road);

/** Usuwa dystans miasta.
 * Zwalnia pamięć zaalokowaną jako dystans i zeruje wskaźnik na nią.
 * Jeśli nie jest zainicjalozowany ten dystans, nic nie robi.
 * @param [in,out] city - wskaźnik na miasto
 */
void deleteDistanceFromCity(City *city);

/** Sprawdza czy nazwa miasta jest poprawna.
 * "Miasto reprezentowane jest przez jego nazwę, która jest niepustym napisem w
 * stylu C niezawierającym kodów od 0 do 31 ani średnika i zakończonym zerem."
 * @param name - wskaźnik na nazwę
 * @return @p true jeśli nazwa jest poprawna, @p false w przeciwnym przypadku
 */
bool validCityName(const char *name);

#endif /* __CITY_H__ */
