#include <stdlib.h>
#include "route.h"
#include "heap.h"
#include "distance.h"



void dijikstra(Vector *cities, City *cityStruct1, City *cityStruct2) {

    Heap *heap = newHeap(cmp);
    for (int i = 0; i < cities->size; i++) {
        insert(heap, createDistance(cities->data[i]));
    }


    cityStruct1->distance->length = 0;
    cityStruct1->distance->oldestBuiltYear = INT_MAX;

    while (!empty(heap)) {
        Distance *distance = pop(heap);
        City *city = distance->city;

        for (int i = 0; i < city->roads->size; i++) {
            Road *road = city->roads->data[i];
            City *nextCity = otherCity(road, city);

            Distance *tmp = sumOfDistanceAndRoad(distance, road);

            if (cmp(nextCity->distance, tmp)) { ///nowa droga jest lepsza


            }

        }

        if (!cmp(distance, minDistatnce->data[cityIndex])) {
            minDistatnce->data[cityIndex] = distance;

        }





    }
}
