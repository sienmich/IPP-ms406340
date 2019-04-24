#ifndef __ROAD_H__
#define __ROAD_H__

#include <stdbool.h>

typedef struct Road Road;

#include "city.h"

typedef struct Road {
    City *city1, *city2;
    unsigned length;
    int builtYear;
} Road;

Road* newRoad(City *city1, City *city2, unsigned length, int builtYear);

void deleteRoad(Road *road);

bool repairRoadFromRoad(Road *Road, int repairYear);

City *otherCity(Road *road, City *city);

char *RoadToString(Road *road);

#endif /* __ROAD_H__ */
