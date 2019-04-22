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
        return NULL;

    ptr->city1 = city1;
    ptr->city2 = city2;
    ptr->length = length;
    ptr->builtYear = builtYear;

    return ptr;
}

void deleteRoad(Road *road) {
    deleteRoadFromCity(road->city1, road);
    deleteRoadFromCity(road->city2, road);
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

