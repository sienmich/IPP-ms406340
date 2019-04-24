#include <stdlib.h>
#include <limits.h>
#include "distance.h"


typedef struct Distance {
    int oldestBuiltYear;
    int length;
    City *city;
} Distance;

Distance *newDistance(City *city) {
    Distance *ptr;
    if (!(ptr = malloc(sizeof(Distance))))
        return NULL;
    ptr->oldestBuiltYear = INT_MIN;
    ptr->length = INT_MAX;
    ptr->city = city;

    return ptr;
}

static void deleteDistance(Distance * distance) {
    free(distance);
}

bool cmp(Distance *a, Distance *b) {
    if (a->length == b->length)
        return a->oldestBuiltYear < b->oldestBuiltYear;
    return a->length > b->length;
}

Distance *sumOfDistanceAndRoad(Distance *a, Road *b) {
    Distance *distance = newDistance(otherCity(b, a->city));

    distance->length = a->length + b->length;
    distance->oldestBuiltYear = max(a->oldestBuiltYear, b->builtYear);

    return distance;
}
