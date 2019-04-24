#include <stdlib.h>
#include <string.h>
#include "city.h"



City *newCity(const char *name) {
    City *ptr;
    if (!(ptr = malloc(sizeof(City))))
        return NULL;

    ptr->distance = NULL;
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

    return ptr;
}

Distance *createDistance(City *city) {
    return city->distance = newDistance(city);
}

void deleteCity(City *city){
    while(city->roads->size > 0) {
        deleteRoad(city->roads->data[0]);
    }
    deleteVector(city->roads);
    deleteDistance(city->distance);
    free(city->name);
    free(city);
}

bool pushBackRoad(City *city, Road *road) {
    return pushBack(roads, road);
}

void deleteRoadFromCity(City *city, Road *road) {
    deleteElementFromVectorBySwap(city->roads, road);
}
