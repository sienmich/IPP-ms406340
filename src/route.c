#include <stdlib.h>
#include "route.h"
#include "heap.h"
#include "distance.h"


Route* newRoute(int idx) {
    Route *ptr;
    if (!(ptr = malloc(sizeof(Route))))
        return NULL;

    if (!(ptr->roads = newVector())) {
        free(ptr);
        return NULL;
    }

    if (!(ptr->cities = newVector())) {
        deleteVector(ptr->roads);
        free(ptr);
        return NULL;
    }

    ptr->idx = idx;

    return ptr;
}


void deleteRoute(Route *route) {
    deleteVector(route->cities);
    deleteVector(route->roads);
    free(route);
}

static intLen(int a) {
    int res = 0;
    a *= 10;    ///a moze long?
    while (a /= 10)
        res++;
    return res;
}



/** @brief Udostêpnia informacje o drodze krajowej.
 * Zwraca wskaŸnik na napis, który zawiera informacje o drodze krajowej. Alokuje
 * pamiêæ na ten napis. Zwraca pusty napis, jeœli nie istnieje droga krajowa
 * o podanym numerze. Zaalokowan¹ pamiêæ trzeba zwolniæ za pomoc¹ funkcji free.
 * Informacje wypisywane s¹ w formacie:
 * numer drogi krajowej;nazwa miasta;d³ugoœæ odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;d³ugoœæ odcinka drogi;rok budowy lub
 * ostatniego remontu;nazwa miasta;…;nazwa miasta.
 * Kolejnoœæ miast na liœcie jest taka, aby miasta @p city1 i @p city2, podane
 * w wywo³aniu funkcji @ref newRoute, które utworzy³o tê drogê krajow¹, zosta³y
 * wypisane w tej kolejnoœci.
 * @param[in,out] map    – wskaŸnik na strukturê przechowuj¹c¹ mapê dróg;
 * @param[in] routeId    – numer drogi krajowej.
 * @return WskaŸnik na napis lub NULL, gdy nie uda³o siê zaalokowaæ pamiêci.
 */
char* RouteToString(Route *route) {
    int len = 3;
    for (int i = 0; i < route->cities->size; i++) {
        City *city = route->cities->data[i];

        len ++;
        len += strlen(city->name);
        if (i < route->roads->size) {
            Road *road = route->roads->data[i];
            len ++;
            len += intLen(road->length);
            len ++;
            len += intLen(road->builtYear);
        }
    }
    len ++;

    char *res;
    if (!(res = malloc(len * sizeof(char))))
        return NULL;

    for (int i = 0; i < 3; i++)
        res[i] = '0' + route->idx / pow(10, 2 - i) % 10;


    int pos = 3;
    for (int i = 0; i < route->cities->size; i++) {
        City *city = route->cities->data[i];

        sprintf(res[pos], ";%s", city->name);
        pos ++;
        pos += strlen(city->name);
        if (i < route->roads->size) {
            Road *road = route->roads->data[i];
            sprintf(res[pos], ";%s;%s", road->length, road->builtYear);
            pos ++;
            pos += intLen(road->length);
            pos ++;
            pos += intLen(road->builtYear);
        }
    }
    pos ++;





}



bool dijikstra(Vector *cities, City *cityStruct1, City *cityStruct2) {

    Heap *heap = newHeap(cmp);
    for (int i = 0; i < cities->size; i++) {
        if(!createDistance(cities->data[i])) {

        }

        insert(heap, createDistance(cities->data[i]));
    }

    cityStruct1->distance->length = 0;
    cityStruct1->distance->oldestBuiltYear = INT_MAX;

    while (!empty(heap)) {
        Distance *distance = pop(heap);
        City *city = distance->city;

        for (int i = 0; i < city->roads->size; i++) {
            Road *road = city->roads->data[i];
            City *nextCity = otherCity(road, city);

            Distance *tmp = sumOfDistanceAndRoad(distance, road);

            if (cmp(nextCity->distance, tmp)) { ///nowa droga jest lepsza


            }

        }

        if (!cmp(distance, minDistatnce->data[cityIndex])) {
            minDistatnce->data[cityIndex] = distance;

        }





    }
}
