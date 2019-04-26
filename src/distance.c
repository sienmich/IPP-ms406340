#include <stdlib.h>
#include <limits.h>
#include "distance.h"


Distance *newDistance(City *city, Road *road) {
    Distance *ptr;
    if (!(ptr = malloc(sizeof(Distance))))
        return NULL;

    ptr->oldestBuiltYear = INT_MAX;
    ptr->length = 0;
    ptr->city = city;
    ptr->road = road;

    return ptr;
}

void deleteDistance(Distance *distance) {
    if (distance)
        free(distance);
}

void deleteDistanceFromCity(City *city) {
    deleteDistance(city->distance);
    city->distance = NULL;
}

bool cmp(Distance *a, Distance *b) {
    if (a->length == b->length)
        return a->oldestBuiltYear < b->oldestBuiltYear;
    return a->length > b->length;
}

Distance *sumOfDistanceAndRoad(Distance *a, Road *b) {
    Distance *distance = newDistance(otherCity(b, a->city), b);
    if (!distance)
        return NULL;

    distance->length = a->length + b->length;
    distance->oldestBuiltYear = a->oldestBuiltYear > b->builtYear ? a->oldestBuiltYear : b->builtYear;

    return distance;
}
















