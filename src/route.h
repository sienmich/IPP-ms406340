#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "vector.h"
#include "city.h"

typedef struct Route {
    Vector *roads;
    Vector *cities;
    int idx;
} Route;

Route* newRoute(int idx);

void deleteRoute(Route *route);



void dijikstra(Vector *cities, City *cityStruct1, City *cityStruct2);

#endif /* __ROUTE_H__ */
