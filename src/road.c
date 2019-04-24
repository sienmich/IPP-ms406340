#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "road.h"
#include "city.h"

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

bool repairRoadFromRoad(Road *road, int repairYear) {
	road->builtYear = repairYear;
	return true;
}

City *otherCity(Road *road, City *city) {
    if (city != road->city1)
        return city1;
    return city2;
}

char *RoadToString(Road *road) {
    char* buffer;
    if(!(buffer = malloc(50 * sizeof(char))))   ///zmien
        return NULL;
    sprintf(buffer, ";%d;%d", road->length, road->builtYear);
    return buffer;
}
