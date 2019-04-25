#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "vector.h"
#include "city.h"

typedef struct Route {
    Vector *roads;
    Vector *cities;
    int idx;
} Route;

Route* nRoute(int idx);

void deleteRoute(Route *route);

char* RouteToString(Route *route);

Route* dijikstra(Vector *cities, City *source, City *target, Road *avoid, Route *avoid2);

bool valid(Route *route);

bool addRoute(Route* route, Route* add, int begin, int end);

Route* bypass(Vector *cities, Route* route, Road *road);

#endif /* __ROUTE_H__ */
