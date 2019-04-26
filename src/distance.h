#ifndef __DISTANCE_H__
#define __DISTANCE_H__

typedef struct Distance Distance;

#include "road.h"
#include "city.h"
#include "route.h"


typedef struct Distance {
    int oldestBuiltYear;
    int length;
    City *city;
    Road *road;
} Distance;


Distance *newDistance(City *city, Road *road);

void clearDistance(Distance *distance, City *city, Road *road);

void deleteDistance(Distance *distance);

void deleteDistanceFromCity(City *city);

bool cmp(Distance *a, Distance *b);

Distance *sumOfDistanceAndRoad(Distance *a, Road *b);


#endif /* __DISTANCE_H__ */
