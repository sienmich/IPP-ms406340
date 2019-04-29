/** @file
 * Implemenrtacja interfejsu klasy miasto
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 28.04.2019
 */
#include <stdlib.h>
#include <string.h>
#include "city.h"

/** Sprawdza czy nazwa miasta jest poprawna.
 * "Miasto reprezentowane jest przez jego nazwę, która jest niepustym napisem w stylu C niezawierającym kodów od 0 do 31 ani średnika i zakończonym zerem."
 * @param name - wskaźnik na nazwę
 * @return @p true jeśli nazwa jest poprawna, @p false w przeciwnym przypadku
 */
static bool validCityName(const char *name) {
    if (!name[0])
        return false;
    for (int i = 0; name[i]; i++) {
        if (name[i] <= 31 && name[i] >= 0)
            return false;
    }
    return true;
}

/** Tworzy nowe miasto.
 * Tworzy nowe miasto o podanej nazwie, które nie posiada żadnych wychodzących dróg.
 * Parametr @p distance nie jest inicjalizowany.
 * @param name [in] - nazwa nowego miasta. Jest kopiowana.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci lub nazwa jest niepoprawna.
 */
City *newCity(const char *name) {
    if (!validCityName(name))
        return NULL;

    City *ptr;
    if (!(ptr = malloc(sizeof(City))))
        return NULL;

    if (!(ptr->roads = newVector())) {
        free(ptr);
        return NULL;
    }

    char *nameCopy;
    if (!(nameCopy = malloc((strlen(name) + 1) * sizeof(char)))) {
        deleteVector(ptr->roads);
        free(ptr);
        return NULL;
    }
    strcpy(nameCopy, name);
    ptr->name = nameCopy;

    ptr->distance = NULL;
    ptr->distance2 = NULL;

    return ptr;
}

/** Usuwa miasto.
 * Usuwa miasto wskazywane przez @p city.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param [in, out] city - wskaźnik na miasto, które usuwa
 */
void deleteCity(City *city){
    while (city->roads->size > 0) {
        deleteRoadUnsafe(city->roads->data[0]);
    }
    deleteVector(city->roads);
    free(city->name);
    free(city);
}

/** Usuwa drogę z miasta.
 * Usuwa drogę z listy sąsiedztwa danego miasta.
 * Jeśli ta droga nie przechodzi przez to miasto, nic nie robi.
 * @param [in,out] city - wskaźnik na miasto
 * @param road - wskaźnik na drogę
 */
void deleteRoadFromCity(City *city, Road *road) {
    deleteElementFromVector(city->roads, road);
}

/** Usuwa dystans miasta.
 * Zwalnia pamięć zaalokowaną jako dystans i zeruje wskaźnik na nią.
 * Jeśli nie jest zainicjalozowany ten dystans, nic nie robi.
 * @param [in,out] city - wskaźnik na miasto
 */
void deleteDistanceFromCity(City *city) {
    deleteDistance(city->distance);
    deleteDistance(city->distance2);
    city->distance = NULL;
    city->distance2 = NULL;
}


