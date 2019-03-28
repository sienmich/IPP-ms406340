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
        exit(1);

    ptr->name = name;
    ptr->roads = newVector();
    ptr->distance = NULL;

    return ptr;
}

Distance *createDistance(City *city) {
    city->distance = newDistance(city);
}


void deleteCity(City *city){
    deleteVector(city->roads);
    deleteDistatnce(city->distance);
    free(city);
}

void addRoad(City *city, Road *road) {
    pushBack(roads, road);
}
