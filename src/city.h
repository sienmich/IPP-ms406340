#ifndef __CITY_H__
#define __CITY_H__

typedef struct City City;

#include <stdbool.h>
#include "vector.h"
#include "distance.h"
#include "road.h"

typedef struct City {
    char *name;
    Vector *roads;
    Distance *distance; ///Do dijikstry
} City;

City* newCity(const char *city);

void deleteCity(City *city);

bool pushBackRoad(City *city, Road *road);

void deleteRoadFromCity(City *city, Road *road);

void deleteDistanceFromCity(City *city);

Distance *createDistance(City *city);

#endif /* __CITY_H__ */
