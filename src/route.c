#include <stdlib.h>

#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "route.h"
#include "heap.h"
#include "distance.h"


Route* nRoute(int idx) {
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
    for (int i = 0; i < route->roads->size; i++) {
        Road *road = route->roads->data[i];
        deleteElementFromVectorBySwap(road->routes, route);
    }
    deleteVector(route->roads);
    free(route);
}

static int intLen(int a) {
    int res = 0;
    a *= 10;    ///a moze long?
    while (a /= 10)
        res++;
    return res;
}

static int RouteToStringHelper(Route *route, char *string, bool print) {
    int len = 0;

    if(print)
        sprintf(string + len, "%d", route->idx);

    len += intLen(route->idx);

    for (int i = 0; i < route->cities->size; i++) {
        City *city = route->cities->data[i];

        if(print)
            sprintf(string + len, ";%s", city->name);
        len ++;
        len += strlen(city->name);

        if (i < route->roads->size) {
            Road *road = route->roads->data[i];
            if(print)
                sprintf(string + len, ";%d;%d", road->length, road->builtYear);
            len ++;
            len += intLen(road->length);
            len ++;
            len += intLen(road->builtYear);
        }
    }
    len ++;
    return len;
}

char* RouteToString(Route *route) {
    char *res;
    int len;

    if (route == NULL) {
        if (!(res = malloc(sizeof(char))))
            return NULL;
        *res = (char) 0;
        return res;
    }

    len = RouteToStringHelper(route, NULL, false);

    if (!(res = malloc(len * sizeof(char))))
        return NULL;

    RouteToStringHelper(route, res, true);

    return res;
}

Route* dijikstra(Vector *cities, City *source, City *target) {
    bool ok = true;

    Heap *heap = newHeap(cmp);
    if (heap == NULL)
        return NULL;

    for (int i = 0; i < cities->size; i++)
        clearDistance(((City *)cities->data[i])->distance, cities->data[i], NULL);

    Distance *start = source->distance;
    start->length = 0;
    start->oldestBuiltYear = INT_MAX;
    if(!insert(heap, start))
        ok = false;

    while (!empty(heap)) {
        Distance *distance = pop(heap);
        City *city = distance->city;
        if (distance != city->distance)
            continue;

        for (int i = 0; i < city->roads->size; i++) {
            Road *road = city->roads->data[i];
            City *nextCity = otherCity(road, city);

            Distance *alt = sumOfDistanceAndRoad(distance, road);
            if(alt == NULL) {
                ok = false;
                break;
            }

            if (cmp(nextCity->distance, alt)) { ///nowa droga jest lepsza
                deleteDistance(nextCity->distance);
                nextCity->distance = alt;
                if(!insert(heap, alt)) {
                    ok = false;
                    break;
                }
            }
        }
    }

    deleteHeap(heap);

    if (!ok)
        return NULL;

    Route *res = nRoute(0);
    if(!res)
        return NULL;


    City *cur = target;
    while (cur != source) {

        if(!pushBack(res->cities, cur))
            ok = false;
        Road *road = cur->distance->road;
        if(!pushBack(res->roads, road))
            ok = false;

        if(!pushBack(road->routes, res))
            ok = false;

        cur = otherCity(road, cur);
    }
    if(!pushBack(res->cities, cur))
        ok = false;


    if (!ok) {
        deleteRoute(res);
        return NULL;
    }


    return res;
}











