#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "road.h"
#include "city.h"

Road* newRoad(City *city1, City *city2, unsigned length, int builtYear) {
	Road *ptr;
    if (!(ptr = malloc(sizeof(Road))))
        return NULL;

    if (!(ptr->routes = newVector())) {
        free(ptr);
        return NULL;
    }

    ptr->city1 = city1;
    ptr->city2 = city2;
    ptr->length = length;
    ptr->builtYear = builtYear;

    return ptr;
}

bool deleteRoad(Road *road, Vector *cities, Vector *routes) {
    Vector *altRoutes = newVector();
    if (!altRoutes)
        return false;

    for (int i = 0; i < road->routes->size; i++) {
        Route *alt = bypass(cities, road->routes->data[i], road);

        if (!alt)
            break;

        if (!pushBack(altRoutes, alt)) {
            deleteRoute(alt);
            break;
        }
    }

    if (road->routes->size != altRoutes->size) {
        for (int i = 0; i < altRoutes->size; i++)
            deleteRoute(altRoutes->data[i]);
        deleteVector(altRoutes);
        return false;
    }

    for (int i = 0; i < altRoutes->size; i++) {
        deleteRoute(road->routes->data[i]);
        Route *r = altRoutes->data[i];
        road->routes->data[i] = r;
        routes->data[r->idx] = r;
    }

    deleteVector(altRoutes);

    deleteRoadUnsafe(road);

	return true;
}

void deleteRoadUnsafe(Road *road) {
    deleteVector(road->routes);
    deleteRoadFromCity(road->city1, road);
    deleteRoadFromCity(road->city2, road);
	free(road);
}

City *otherCity(Road *road, City *city) {
    if (city != road->city1)
        return road->city1;
    return road->city2;
}
