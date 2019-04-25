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

Route* dijikstra(Vector *cities, City *source, City *target, Road *avoid, Route *avoid2) {
    bool ok = true;

    Heap *heap = newHeap(cmp);
    if (heap == NULL)
        return NULL;

    for (int i = 0; i < cities->size; i++)
        clearDistance(((City *)cities->data[i])->distance, cities->data[i], NULL);

    if (avoid2)
        for (int i = 0; i < avoid2->cities->size; i++) {
            City *c = avoid2->cities->data[i];
            if (c != source && c != target)
                c->distance->length = 0;
        }


    Distance *start = target->distance;
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

            if (road == avoid)
                continue;

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


    City *cur = source;
    while (cur != target) {

        if(!pushBack(res->cities, cur))
            ok = false;
        Road *road = cur->distance->road;
        if (!road) {
            ok = false;
            break;
        }

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

bool valid(Route *route) {
return true;
    for (int i = 0; i < route->cities->size; i++) {
        City *city = route->cities->data[i];
        clearDistance(city->distance, city, NULL);
    }
    for (int i = 0; i < route->cities->size; i++) {
        City *city = route->cities->data[i];
        if (!city->distance->length)
            return false;
        city->distance->length = 0;
    }
    return true;

}

bool addRoute(Route* route, Route* add, int begin, int end) {
    bool ok = true;
    int cs = route->cities->size;
    int rs = route->roads->size;
    for (int i = begin; i < end; i++) {

        if(!pushBack(route->cities, add->cities->data[i]))
            ok = false;

        Road *r = add->roads->data[i];
        if(!pushBack(route->roads, r))
            ok = false;

        else if(!pushBack(r->routes,  route))
            ok = false;
    }
    if (!ok) {
        while (cs != route->cities->size)
            popBack(route->cities);
        while (rs != route->roads->size) {
            Road *r = popBack(route->roads);
            deleteElementFromVectorBySwap(r->routes, route);
        }
        return false;
    }
    return true;
}


Route* bypass(Vector *cities, Route* route, Road *road) {

    Route *full = nRoute(route->idx);
    if(!full)
        return NULL;

    int i = 0;
    while (route->roads->data[i] != road)
        i++;

    if(!addRoute(full, route, 0, i)) {
        deleteRoute(full);
        return NULL;
    }

    City *city1 = route->cities->data[i];
    City *city2 = route->cities->data[i + 1];
    Route *alt = dijikstra(cities, city1, city2, road, route);

    if(!alt) {
        deleteRoute(full);
        return NULL;
    }

    if(!addRoute(full, alt, 0, alt->roads->size)) {
        deleteRoute(alt);
        deleteRoute(full);
        return NULL;
    }
    deleteRoute(alt);


    if(!addRoute(full, route, i + 1, route->roads->size)) {
        deleteRoute(full);
        return NULL;
    }

    pushBack(full->cities, route->cities->data[route->cities->size - 1]);

    if (!valid(full)) {
        deleteRoute(full);
        return NULL;
    }

    return full;
}



