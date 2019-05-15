/** @file
 * Implementacja interfejsu klasy przechowującej mapę dróg krajowych.
 *
 * @author Łukasz Kamiński <kamis@mimuw.edu.pl>, Marcin Peczarski <marpe@mimuw.edu.pl>,
 * Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 29.04.2019
 */
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include "map.h"
#include "city.h"
#include "road.h"
#include "vector.h"
#include "route.h"
#include "stringVector.h"

/** Stała określająca maksymalny numer trasy
 */
#define ROUTES_SIZE 1000

/**
 * Struktura przechowująca mapę dróg krajowych.
 */
typedef struct Map {
    Vector *cities; ///< Wskaźnik na wektor zawierający wszystkie miasta
    Vector *routes; ///< Wskaźnik na wektor zawierający wszystkie trasy
} Map;


/** @brief Tworzy nową strukturę.
 * Tworzy nową, pustą strukturę niezawierającą żadnych miast, odcinków dróg ani
 * dróg krajowych.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Map *newMap(void) {
    Map *ptr;
    if (!(ptr = malloc(sizeof(Map))))
        return NULL;

    if (!(ptr->cities = newVector())) {
        free(ptr);
        return NULL;
    }

    if (!(ptr->routes = newVectorWithSize(ROUTES_SIZE))) {
        deleteVector(ptr->cities);
        free(ptr);
        return NULL;
    }

    for (int i = 0; i < ROUTES_SIZE; i++) {
        ptr->routes->data[i] = NULL;
    }

    return ptr;
}

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p map.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param[in] map        – wskaźnik na usuwaną strukturę.
 */
void deleteMap(Map *map) {
    if (map == NULL)
        return;

    for (int i = 0; i < map->routes->size; i++)
        deleteRoute(map->routes->data[i]);
    deleteVector(map->routes);

    for (int i = 0; i < map->cities->size; i++)
        deleteCity(map->cities->data[i]);
    deleteVector(map->cities);

    free(map);
}

/** Dodaje do mapy nowe miasto.
 * @param[out] map - wskaźnik na mapę
 * @param[in] city - wskaźnik na napis będący nawą miasta
 * @return Wartość @p true, jeśli miasto zostało dodane.
 * Wartość @p false, jeśli wystąpił błąd: nie udało się zaalokować pamięci.
 */
static bool addCity(Map *map, const char *city) {
    City *cityStruct = newCity(city);
    if (!cityStruct) {
        return false;
    }
    if (!pushBack(map->cities, cityStruct)) {
        deleteCity(cityStruct);
        return false;
    }
    return true;
}

/** Znajduje na mapie miasto znając jego nazwę.
 * @param[in] map - wskaźnik na mapę
 * @param[in] city - wskaźnik na napis będący nawą miasta
 * @return Wskaźnik na znalezione miasto lub NULL, gdy nie ma takiego miasta.
 */
static City *findCityFromString(Map *map, const char *city) {
    for (int i = 0; i < map->cities->size; i++) {
        City *elem = map->cities->data[i];
        if (!strcmp(city, elem->name))
            return elem;
    }
    return NULL;
}

/** Znajduje lub dodaje miasto na mapie.
 * Jeśli nie istnieje miasto o podanej nazwie, tworzy je.
 * @param[in,out] map - wskaźnik na mapę
 * @param[in] city - wskaźnik na napis będący nawą miasta
 * @return Wskaźnik na miasto o podanej nazwie lub NULL,
 * gdy nie udało się zaalokować pamięci.
 */
static City *findCityFromStringOrAdd(Map *map, const char *city) {
    City *res = findCityFromString(map, city);
    if (res == NULL) {
        if (addCity(map, city))
            return map->cities->data[map->cities->size - 1];
        return NULL;
    }
    return res;
}

/** Znajduje drogę łączącą dwa miasta.
 * @param[in] city1 - wskaźnik na pierwsze miasto
 * @param[in] city2 - wskaźnik na drugie miasto
 * @return Wskaźnik na drogę łączącą te miasta lub NULL, gdy taka droga nie istenieje.
 */
static Road *findRoadFromCities(City *city1, City *city2) {
    for (int i = 0; i < city1->roads->size; i++) {
        Road *road = city1->roads->data[i];
        if (road->city1 == city2 || road->city2 == city2)
            return road;
    }
    return NULL;
}

/** @brief Dodaje do mapy odcinek drogi między dwoma różnymi miastami.
 * Jeśli któreś z podanych miast nie istnieje, to dodaje go do mapy, a następnie
 * dodaje do mapy odcinek drogi między tymi miastami.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] length     – długość w km odcinka drogi;
 * @param[in] builtYear  – rok budowy odcinka drogi.
 * @return Wartość @p true, jeśli odcinek drogi został dodany.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, obie podane nazwy miast są identyczne, odcinek drogi między tymi
 * miastami już istnieje lub nie udało się zaalokować pamięci.
 */
bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear) {
    if (!length || !builtYear)
        return false;

    City *cityStruct1 = findCityFromStringOrAdd(map, city1);
    City *cityStruct2 = findCityFromStringOrAdd(map, city2);
    if (!cityStruct1 || !cityStruct2 || cityStruct1 == cityStruct2)
        return false;

    if (findRoadFromCities(cityStruct1, cityStruct2))
        return false;

    Road *road = newRoad(cityStruct1, cityStruct2, length, builtYear);

    if (!road)
        return false;

    if (!pushBack(cityStruct1->roads, road)) {
        deleteRoadUnsafe(road);
        return false;
    }
    if (!pushBack(cityStruct2->roads, road)) {
        popBack(cityStruct1->roads);
        deleteRoadUnsafe(road);
        return false;
    }

    return true;
}

/** Znajduje drogę łączącą dwa miasta o podanych nazwach.
 * @param[in] map - wskaźnik na mapę
 * @param[in] city1 - wskaźnik na nazwę pierwszego miasto
 * @param[in] city2 - wskaźnik na nazwę drugiego miasto
 * @return Wskaźnik na drogę łączącą te miasta lub NULL,
 * gdy któreś z tych miast nie istnieje lub taka droga nie istenieje.
 */
static Road *findRoadFromStrings(Map *map, const char *city1, const char *city2) {
    City *cityStruct1 = findCityFromString(map, city1);
    City *cityStruct2 = findCityFromString(map, city2);

    if (!cityStruct1 || !cityStruct2 || cityStruct1 == cityStruct2)
        return NULL;

    return findRoadFromCities(cityStruct1, cityStruct2);
}

/** @brief Modyfikuje rok ostatniego remontu odcinka drogi.
 * Dla odcinka drogi między dwoma miastami zmienia rok jego ostatniego remontu
 * lub ustawia ten rok, jeśli odcinek nie był jeszcze remontowany.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] repairYear – rok ostatniego remontu odcinka drogi.
 * @return Wartość @p true, jeśli modyfikacja się powiodła.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, któreś z podanych miast nie istnieje, nie ma odcinka drogi między
 * podanymi miastami, podany rok jest wcześniejszy niż zapisany dla tego odcinka
 * drogi rok budowy lub ostatniego remontu.
 */
bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
    if (!repairYear)
        return false;

    Road *road = findRoadFromStrings(map, city1, city2);
    if (road != NULL)
        if (road->builtYear <= repairYear) {
            road->builtYear = repairYear;
            return true;
        }

    return false;
}

/** Znajduje drogę lub ją dodaje.
 * Jeśli istnieje już droga łączące podane dwa miasta, to naprawia ją i zwraca.
 * Jeśli nie ma takiej drogi to ją tworzy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] length     – długość drogi;
 * @param[in] repairYear – rok ostatniego remontu odcinka drogi.
 * @return Wskaźnik na szukaną drogę lub NULL, gdy wystąpił błąd:
 * Nieudana alokacja pamięci lub istnieje droga między tymi miastami
 * o innej długości lub późniejszym roku budowy / remontu.
 */
static Road *findRoadOrAdd(Map *map, const char *city1, const char *city2,
                           int length, int repairYear) {
    Road *r = findRoadFromStrings(map, city1, city2);
    if (r) {
        if ((int) r->length != length)
            return NULL;
        if (!repairRoad(map, city1, city2, repairYear))
            return NULL;
    }
    if (!r) {
        if (!addRoad(map, city1, city2, length, repairYear))
            return NULL;
    }
    return findRoadFromStrings(map, city1, city2);
}


/** Tworzy nową trasę na podstawie opisu.
 * Opis to ciąg słów postaci "numer drogi krajowej;nazwa miasta;
 * długość odcinka drogi;rok budowy lub ostatniego remontu;nazwa miasta;
 * długość odcinka drogi;rok budowy lub ostatniego remontu;nazwa miasta;
 * …;nazwa miasta".
 *
 * Tworzy drogę krajową o podanym numerze i przebiegu.
 * Jeśli jakieś miasto lub odcinek drogi nie istnieje, to go tworzy.
 * Jeśli odcinek drogi już istnieje, ale ma wcześniejszy rok budowy
 * lub ostatniego remontu, to modyfikuje ten atrybut odcinka drogi.
 * Za błąd uznajemy, jeśli odcinek drogi już istnieje, ale ma inną
 * długość albo późniejszy rok budowy lub ostatniego remontu.
 * To polecenie niczego nie wypisuje na standardowe wyjście.
 *
 * W przypadku wystąpienia błędu część dróg może zostać dodana na mapę.
 *
 * @param[in,out] map - wskaźnik na mapę
 * @param[in] description - wskaźnik na wektor stringów
 * @return @p false gdy wystąpił bład lub nieudało się zaalokować pamięci.
 * @p true w przeciwnym przypadku.
 */
bool newRouteFromDescription(Map *map, Vector *description) {
    if (description->size % 3 != 2)
        return false;

    int routeId = toInt(description->data[0]);
    if (routeId < 1 || routeId >= ROUTES_SIZE)
        return false;
    if (map->routes->data[routeId])
        return false;

    for (int i = 1; i < description->size; i++) {
        if (i % 3 == 1)
            if (!validCityName(toCharArray(description->data[i])))
                return false;
        if (i % 3 == 0 || i % 3 == 2)
            if (!toInt(description->data[i]))
                return false;
    }

    Route *res = nRoute(routeId);
    bool ok = true;

    char *lastCity = toCharArray(description->data[1]);

    for (int i = 2; i < description->size && ok; i += 3) {
        int length = toInt(description->data[i]);
        int year = toInt(description->data[i + 1]);
        char *nextCity = toCharArray(description->data[i + 2]);

        Road *road = findRoadOrAdd(map, lastCity, nextCity, length, year);

        if (!road || !pushBack(res->roads, road))
            ok = false;

        else if (!pushBack(res->cities, findCityFromString(map, lastCity)))
            ok = false;

        lastCity = nextCity;
    }


    if (!pushBack(res->cities, findCityFromString(map, lastCity)))
        ok = false;

    if (!ok) {
        deleteRoute(res);
        return false;
    }

    map->routes->data[routeId] = res;
    return true;
}

/** @brief Usuwa odcinek drogi między dwoma różnymi miastami.
 * Usuwa odcinek drogi między dwoma miastami. Jeśli usunięcie tego odcinka drogi
 * powoduje przerwanie ciągu jakiejś drogi krajowej, to uzupełnia ją
 * istniejącymi odcinkami dróg w taki sposób, aby była najkrótsza. Jeśli jest
 * więcej niż jeden sposób takiego uzupełnienia, to dla każdego wariantu
 * wyznacza wśród dodawanych odcinków drogi ten, który był najdawniej wybudowany
 * lub remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli odcinek drogi został usunięty.
 * Wartość @p false, jeśli z powodu błędu nie można usunąć tego odcinka drogi:
 * któryś z parametrów ma niepoprawną wartość, nie ma któregoś z podanych miast,
 * nie istnieje droga między podanymi miastami, nie da się jednoznacznie
 * uzupełnić przerwanego ciągu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool removeRoad(Map *map, const char *city1, const char *city2) {
    Road *road = findRoadFromStrings(map, city1, city2);
    if (road != NULL) {
        return deleteRoad(road, map->cities, map->routes);
    }
    return false;
}

/** @brief Łączy dwa różne miasta drogą krajową.
 * Tworzy drogę krajową pomiędzy dwoma miastami i nadaje jej podany numer.
 * Wśród istniejących odcinków dróg wyszukuje najkrótszą drogę. Jeśli jest
 * więcej niż jeden sposób takiego wyboru, to dla każdego wariantu wyznacza
 * wśród wybranych w nim odcinków dróg ten, który był najdawniej wybudowany lub
 * remontowany i wybiera wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city1      – wskaźnik na napis reprezentujący nazwę miasta;
 * @param[in] city2      – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została utworzona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * wartość, istnieje już droga krajowa o podanym numerze, któreś z podanych
 * miast nie istnieje, obie podane nazwy miast są identyczne, nie można
 * jednoznacznie wyznaczyć drogi krajowej między podanymi miastami lub nie udało
 * się zaalokować pamięci.
 */
bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2) {
    if (routeId < 1 || routeId >= ROUTES_SIZE)
        return false;

    City *cityStruct1 = findCityFromString(map, city1);
    City *cityStruct2 = findCityFromString(map, city2);
    if (cityStruct1 == NULL || cityStruct2 == NULL || cityStruct1 == cityStruct2)
        return false;

    if (map->routes->data[routeId])
        return false;

    Route *res = dijikstraOnlyOne(map->cities, cityStruct1, cityStruct2, NULL, NULL);
    if (res) {
        res->id = routeId;
        map->routes->data[routeId] = res;
        return true;
    }

    return false;
}

/** @brief Wydłuża drogę krajową do podanego miasta.
 * Dodaje do drogi krajowej nowe odcinki dróg do podanego miasta w taki sposób,
 * aby nowy fragment drogi krajowej był najkrótszy. Jeśli jest więcej niż jeden
 * sposób takiego wydłużenia, to dla każdego wariantu wyznacza wśród dodawanych
 * odcinków dróg ten, który był najdawniej wybudowany lub remontowany i wybiera
 * wariant z odcinkiem, który jest najmłodszy.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej;
 * @param[in] city       – wskaźnik na napis reprezentujący nazwę miasta.
 * @return Wartość @p true, jeśli droga krajowa została wydłużona.
 * Wartość @p false, jeśli wystąpił błąd: któryś z parametrów ma niepoprawną
 * nazwę, nie istnieje droga krajowa o podanym numerze, nie ma miasta o podanej
 * nazwie, przez podane miasto już przechodzi droga krajowa o podanym numerze,
 * podana droga krajowa kończy się w podanym mieście, nie można jednoznacznie
 * wyznaczyć nowego fragmentu drogi krajowej lub nie udało się zaalokować
 * pamięci.
 */
bool extendRoute(Map *map, unsigned routeId, const char *city) {
    if (routeId < 1 || routeId >= ROUTES_SIZE)
        return false;

    City *cityStruct = findCityFromString(map, city);
    if (!cityStruct)
        return false;

    Route *r = map->routes->data[routeId];
    if (!r)
        return false;

    for (int i = 0; i < r->cities->size; i++)
        if (r->cities->data[i] == cityStruct)
            return false;

    Route *alt = extendBoth(map->cities, r, cityStruct);
    if (!alt)
        return false;

    deleteRoute(r);
    map->routes->data[routeId] = alt;
    return true;
}

/** @brief Udostępnia informacje o drodze krajowej.
 * Zwraca wskaźnik na napis, który zawiera informacje o drodze krajowej. Alokuje
 * pamięć na ten napis. Zwraca pusty napis, jeśli nie istnieje droga krajowa
 * o podanym numerze. Zaalokowaną pamięć trzeba zwolnić za pomocą funkcji free.
 * Informacje wypisywane są w formacie:
 * numer drogi krajowej;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;długość odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;…;nazwa miasta.
 * Kolejność miast na liście jest taka, aby miasta @p city1 i @p city2, podane
 * w wywołaniu funkcji @ref newRoute, które utworzyło tę drogę krajową, zostały
 * wypisane w tej kolejności.
 * @param[in,out] map    – wskaźnik na strukturę przechowującą mapę dróg;
 * @param[in] routeId    – numer drogi krajowej.
 * @return Wskaźnik na napis lub NULL, gdy nie udało się zaalokować pamięci.
 */
char const *getRouteDescription(Map *map, unsigned routeId) {
    if (routeId < 1 || routeId >= ROUTES_SIZE) {
        char *res;
        if (!(res = malloc(sizeof(char))))
            return NULL;
        *res = (char) 0;
        return res;
    }

    return RouteToString(map->routes->data[routeId]);
}

