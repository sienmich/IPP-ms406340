
#include <string.h>
#include "map.h"
#include "vector.h"
#include "route.h"

typedef struct Map {
    vector *cities;
//    vector *roads;
} Map;


Map* newMap(void) {
    Map *ptr;
    if (!(ptr = malloc(sizeof(Map))))
        return NULL;

    if (!(ptr->cities = newVector())) {
        free(ptr);
        return NULL;
    }
    /*
    if (!(ptr->roads = newVector())) {
        deleteVector(ptr->cities);
        free(ptr);
        return NULL;
    }
    */

	return ptr;
}

void deleteMap(Map *map) {
	if (map == NULL)
        return;

    while(map->cities->size > 0) {
        deleteCity(map->cities->data[0]);
        deleteElementFromVectorBySwap(map->cities, map->cities->data[0]);
    }

    deleteVector(map->cities);
 //   deleteVector(map->roads);
    free(map);
}

static bool addCity(Map *map, const char *city) {
    City *cityStruct = newCity(city);
    if(!cityStruct)
        return false;
    if(!pushBack(map->cities, cityStruct)) {
        deleteCity(cityStruct);
        return false;
    }
    return true;
}

/// jak nie ma, to NULL
static City* findCityFromString(Map *map, const char *city) {
    for (int i = 0; i < map->cities->size; i++)
    {
        City *elem = map->cities->data[i];
        if (!strcmp(city, elem->name))
            return elem;
    }
    return NULL;
}

/// jak nie ma, to tworzy nowy
static City* findCityFromStringOrAdd(Map *map, const char *city) {
    City* res = findCityFromString(map, city);
    if (res == NULL) {
        if (addCity(map, city))
            return data[map->cities->size - 1];
        return NULL;
    }
    return res;
}

/// jak nie ma, to NULL
static City* findRoadFromCities(Map *map, City *city1, City *city2) {
    for (int i = 0; i < city1->roads->size; i++)
    {
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

    City *cityStruct1 = findCityFromStringOrAdd(map, city1);    ///jest szansa, ze dodam miasto i nie cofne tego. trudno
    City *cityStruct2 = findCityFromStringOrAdd(map, city2);
    if(!cityStruct1 || !cityStruct2 || cityStruct1 == cityStruct2)
        return false;

    if(findRoadFromCities(cityStruct1, cityStruct2))
        return false;

    Road *road = newRoad(cityStruct1, cityStruct2, length, builtYear);

    if(!road)
        return false;

//    if(!pushBack(map->roads, road)) {
//       deleteRoad(road);
//        return false;
//    }
    if(!pushBack(cityStruct1->roads, road)) {
//        popBack(map->roads);
        deleteRoad(road);
        return false;
    }
    if(!pushBack(cityStruct2->roads, road)) {
//        popBack(map->roads);
        popBack(cityStruct1->roads);
        deleteRoad(road);
        return false;
    }
    return true;
}

static Road* findRoadFromStrings(const char *city1, const char *city2) {

    City *cityStruct1 = findCityFromString(map, city1);
    City *cityStruct2 = findCityFromString(map, city2);

    if(!cityStruct1 || !cityStruct2 || cityStruct1 == cityStruct2)
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
    Road *road = findRoadFromStrings(city1, city2);
    if (road != NULL)
        if (road->builtYear <= repairYear) {
            road->builtYear = repairYear;
            return true;
        }

    return false;
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
    Road *road = findRoadFromStrings(city1, city2);
    if (road != NULL) {
        deleteRoad(road);
        return true;
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
    City *cityStruct1 = findCityFromString(map, city1);
    City *cityStruct2 = findCityFromString(map, city2);
    if (cityStruct1 == NULL || cityStruct1 == NULL)
        return false;

    dijikstra(map->cities, cityStruct1, cityStruct2);


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
bool extendRoute(Map *map, unsigned routeId, const char *city);



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
char const* getRouteDescription(Map *map, unsigned routeId);
