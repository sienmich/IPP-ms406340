#include <limits.h>
#include "route.h"

typedef struct Distance {
    int oldestBuiltYear;
    int length;
    City *city;
} Distatnce;

Distatnce *newDistance(City *city) {
    Distatnce *ptr;
    if (!(ptr = malloc(sizeof(Distatnce))))
        return NULL;
    ptr->oldestBuiltYear = INT_MIN;
    ptr->length = INT_MAX;
    ptr->city = city;

    return ptr;
}

static void deleteDistatnce(Distatnce * distatnce) {
    free(distatnce);
}

bool cmp(Distance *a, Distance *b) {
    if (a->length == b->length)
        return a->oldestBuiltYear < b->oldestBuiltYear;
    return a->length > b->length;
}

Distatnce *sumOfDistanceAndRoad(Distance *a, Road *b) {
    Distance *distance = newDistance(otherCity(b, a->city));

    distance->length = a->length + b->length;
    distance->oldestBuiltYear = max(a->oldestBuiltYear, b->builtYear);

    return distance;
}
