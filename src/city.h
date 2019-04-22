#ifndef __CITY_H__
#define __CITY_H__

#include <stdbool.h>

typedef struct City City;

City* newCity(const char *city);

void deleteCity(City *city);

void pushBackRoad(City *city, Road *road);

void deleteRoadFromCity(City *city, Road *road);

Distance *createDistance(City *city);

#endif /* __CITY_H__ */
