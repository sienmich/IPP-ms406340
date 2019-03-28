#ifndef __ROAD_H__
#define __ROAD_H__

#include <stdbool.h>

typedef struct Road Road;

Road* newRoad(City *city1, City *city2, unsigned length, int builtYear);

void deleteRoad(Road *road);

bool repairRoad(Road *Road, int repairYear);

City *otherCity(Road *road, City *city);


#endif /* __ROAD_H__ */
