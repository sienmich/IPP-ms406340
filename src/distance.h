/** @file
 * Interfejs klasy dynstans opisującej odległość między dwoma miastami
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 28.04.2019
 */
#ifndef __DISTANCE_H__
#define __DISTANCE_H__

typedef struct Distance Distance;

#include "road.h"
#include "city.h"


/** @brief Odległość od źródła do danego miasta.
 * Wykorzystawana w algytmie Dijikstry.
*/
typedef struct Distance {
    int length; ///< Długość najkrótszej trasy
    int oldestBuiltYear; ///< Najstarszy rok wybudowania lub ostatniego remontu na trasie
    City *city; ///< Wskaźnik na miasto, którego dystans przechowuje
    Road *road; ///< Wskaźnik na ostatni odcinek drogi potrzebny by uzyskać ten dystans
} Distance;

/** @brief Tworzy nowy dystans.
 * Tworzy nowy dystans, który opisuje miasto @p city, a ostatnia droga to @p road.
 * @p lenght ustawia na 0, a @p oldestBuiltYear na MAX_INT
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Distance *newDistance(City *city, Road *road);

/** @brief Usuwa dystans.
 * Usuwa dystans wskazywany przez @p distance.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param [in, out] distance - wskaźnik na dystans, który usuwa
 */
void deleteDistance(Distance *distance);

/** @brief Porównuje dwa dystansy.
 * Dystans @p a jest mniejszy niż @p b, jeśli ma mniejszą długość lub większy najstarszy rok i równe długości.
 * @param [in] a - wskaźnik na pierwszy dystans
 * @param [in] b - wskaźnik na drugi dystans
 * @return @p true jeśli dystans @p a jest większy niż dystans @p b, @p false w przeciwnym przypadku
 */
bool cmpDistance(Distance *a, Distance *b);

/** @brief Dodaje drogę do dystansu.
 * Tworzy nowy dystans, który opisuje trasę opisującą dystans @p a z dodaną na koniec drogą @p b.
 * Przez to trasa prowadzi do następnego miasta, ma większą długość.
 * Należy w przyszłości zwolnić zadeklarowaną pamięć za pomocą @p deleteDistance.
 * @param [in] a - wskaźnik na dystans
 * @param [in] b - wskaźnik na drogę
 * @return Wskaźnik na nowy dystans lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Distance *sumOfDistanceAndRoad(Distance *a, Road *b);


#endif /* __DISTANCE_H__ */
