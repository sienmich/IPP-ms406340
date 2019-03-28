#include <limits.h>
#include "route.h"

static typedef static struct Distance {
    int oldestBuiltYear;
    int length;
    int cityIndex;
} Distatnce;

static Distatnce *newDistance(int cityIndex) {
    Distatnce *ptr;
    if (!(ptr = malloc(sizeof(Distatnce))))
        exit(1);
    ptr->oldestBuiltYear = INT_MIN;
    ptr->length = INT_MAX;
    ptr->cityIndex = cityIndex;

    return ptr;
}

static void deleteDistatnce(Distatnce * distatnce) {
    free(distatnce);
}

bool cmp(Distance *a, Distance *b) {
    if (a->length == b->length)
        return a->oldestBuiltYear < b->oldestBuiltYear;
    return a->length > b->length;

}


void dijikstra(Vector *cities, Vector *roads, City *cityStruct1, City *cityStruct2) {


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
