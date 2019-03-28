#ifndef __DISTANCE_H__
#define __DISTANCE_H__



typedef struct Distance Distatnce;

Distatnce *newDistance(City *city);

void deleteDistatnce(Distatnce * distatnce);

bool cmp(Distance *a, Distance *b);

Distatnce *sumOfDistanceAndRoad(Distance *a, Road *b);


#endif /* __DISTANCE_H__ */
