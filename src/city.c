#include "city.h"
#include "road.h"

typedef struct City {
	char *name;

    int size;
    int last;
	Road **roads;
} City;

City *newCity(const char *name) {
    City *ptr;
    if (!(ptr = malloc(sizeof(City))))
        exit(1);
        
    ptr->size = 0;
    ptr->last = 0;

    return ptr;
}

void deleteCity(City *city){
	free(city->roads);
    free(city);
}

static void extendVector(City *city) {
	city->size = city->size * 2;
	if (city->size == 0)
		city->size = 1;
	
	if (!(city->roads = realloc(city->roads, city->size * sizeof(*Road))))
            exit(1);
}

void pushBackRoad(City *city, Road *road) {
	if (city->last == city->size)
		extendVector(city);
	city->roads[city->last++] = road;
}
