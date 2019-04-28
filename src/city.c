#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "city.h"

static bool validCityName(const char *name) {
    if (!name[0])
        return false;
    for (int i = 0; name[i]; i++) {
        if (name[i] <= 31 && name[i] >= 0)
            return false;
    }
    return true;
}

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

    return ptr;
}

void deleteCity(City *city){
    while(city->roads->size > 0) {
        deleteRoadUnsafe(city->roads->data[0]);
    }
    deleteVector(city->roads);
    free(city->name);
    free(city);
}

void deleteRoadFromCity(City *city, Road *road) {
    deleteElementFromVector(city->roads, road);
}

void deleteDistanceFromCity(City *city) {
    deleteDistance(city->distance);
    city->distance = NULL;
}


