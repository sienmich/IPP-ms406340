#include "city.h"
#include "road.h"
#include "vector.h"

typedef struct City {
	char *name;
    Vector *roads;
    Distance *distance; ///Do dijikstry
} City;

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
        deleteElementFromVectorBySwap(city->roads, city->roads->data[0]);
    }
    deleteVector(city->roads);
    deleteDistatnce(city->distance);
    free(city->name);
    free(city);
}

bool pushBackRoad(City *city, Road *road) {
    return pushBack(roads, road);
}

void deleteRoadFromCity(City *city, Road *road) {
    deleteElementFromVectorBySwap(city->roads, road);
}
