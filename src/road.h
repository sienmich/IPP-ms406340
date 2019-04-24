#ifndef __ROAD_H__
#define __ROAD_H__

#include <stdbool.h>

typedef struct Road Road;

#include "city.h"
#include "vector.h"

typedef struct Road {
    City *city1, *city2;
    unsigned length;
    int builtYear;
    Vector *routes;
} Road;

Road* newRoad(City *city1, City *city2, unsigned length, int builtYear);

void deleteRoad(Road *road);

bool repairRoadFromRoad(Road *Road, int repairYear);

City *otherCity(Road *road, City *city);

#endif /* __ROAD_H__ */
