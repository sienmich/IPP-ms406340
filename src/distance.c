#include <stdlib.h>
#include <limits.h>
#include "distance.h"


void clearDistance(Distance *distance, City *city, Road *road) {
    distance->oldestBuiltYear = INT_MIN;
    distance->length = INT_MAX;
    distance->city = city;
    distance->road = road;
}

Distance *newDistance(City *city, Road *road) {
    Distance *ptr;
    if (!(ptr = malloc(sizeof(Distance))))
        return NULL;

    clearDistance(ptr, city, road);
    return ptr;
}

void deleteDistance(Distance *distance) {
    free(distance);
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
















