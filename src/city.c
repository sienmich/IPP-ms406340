#include "city.h"
#include "road.h"
#include "vector.h"

typedef struct City {
	char *name;
    vector *roads;
} City;

City *newCity(const char *name) {
    City *ptr;
    if (!(ptr = malloc(sizeof(City))))
        exit(1);

    ptr->name = name;
    ptr->roads = newVector();

    return ptr;
}

void deleteCity(City *city){
    deleteVector(city->roads);
    free(city);
}

void addRoad(City *city, Road *road) {
    pushBack(roads, road);
}
