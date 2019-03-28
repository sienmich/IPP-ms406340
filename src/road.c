#include <stdbool.h>
#include "road.h"
#include "city.h"

typedef struct Road {
	City *city1, *city2;
	unsigned length;
	int builtYear;
} Road;

Road* newRoad(City *city1, City *city2, unsigned length, int builtYear) {
	Road *ptr;
    if (!(ptr = malloc(sizeof(Road))))
        exit(1);

    ptr->city1 = city1;
    ptr->city2 = city2;
    ptr->length = length;
    ptr->builtYear = builtYear;

    return ptr;
}

void deleteRoad(Road *road) {
	free(road);
}

bool repairRoad(Road *road, int repairYear) {
	road->builtYear = repairYear;
	return true;
}

City *otherCity(Road *road, City *city) {
    if (city != road->city1)
        return city1;
    return city2;
}

