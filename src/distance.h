#ifndef __DISTANCE_H__
#define __DISTANCE_H__

typedef struct Distance Distance;

#include "road.h"
#include "city.h"
#include "route.h"

Distance *newDistance(City *city);

void deleteDistance(Distance *distance);

bool cmp(Distance *a, Distance *b);

Distance *sumOfDistanceAndRoad(Distance *a, Road *b);


#endif /* __DISTANCE_H__ */
