/** @file
 * Implementacja interfejsu klasy trasa przechowywującej drogi krajowe
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 28.04.2019
 */
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "route.h"
#include "heap.h"
#include "distance.h"

/** Tworzy nową trasę.
 * Tworzy nową trasę, która ma pustą listę miast i dróg.
 * @param[in] id - numer trasy
 * @return Wskaźnik na nową strukturę lub NULL,
 * gdy nie udało się zaalokować pamięci.
 */
Route *nRoute(int id) {
    Route *ptr;
    if (!(ptr = malloc(sizeof(Route))))
        return NULL;

    if (!(ptr->roads = newVector())) {
        free(ptr);
        return NULL;
    }

    if (!(ptr->cities = newVector())) {
        deleteVector(ptr->roads);
        free(ptr);
        return NULL;
    }

    ptr->id = id;

    return ptr;
}

/** Usuwa trasę.
 * Zwalnia pamięć zaalokowaną pod trasę.
 * Dodatkowo usuwa wskaźnik na tę trasę ze wszystkich dróg
 * na niej występujących.
 * Nic nie robi, gdy wskaźnik jest równy NULL.
 * @param[in,out] route - wskaźnik na usuwaną strukturę
 */
void deleteRoute(Route *route) {
    if (!route)
        return;
    deleteVector(route->cities);
    for (int i = 0; i < route->roads->size; i++) {
        Road *road = route->roads->data[i];
        deleteElementFromVector(road->routes, route);
    }
    deleteVector(route->roads);
    free(route);
}

/** Liczy długość liczby w zapisie dziesiętnym.
 * Jeśli jest ujemna, jest dłuższa o 1.
 * @param[in] a - liczba
 * @return Długość liczby.
 */
static int intLen(int a) {
    long long res = 0;
    if (a < 0)
        res++;
    a *= 10;    ///a moze long?
    while (a /= 10)
        res++;
    return res;
}

/** Pomocnicza funkcja @ref RouteToString.
 * Zwraca długość całego opisu trasy.
 * Zapisuje informacje o trasie pod adresem @p string.
 * Napis jest zgodny ze specyfikacją @ref getRouteDescription.
 * @param[in] route - wskaźnik na trasę
 * @param[out] string - wskaźnik na napis
 * @param[in] print - czy ma wypisywać trasę do @p string
 * @return Dłogość całego opisu trasy.
 */
static int RouteToStringHelper(Route *route, char *string, bool print) {
    int len = 0;

    if (print)
        sprintf(string + len, "%d", route->id);

    len += intLen(route->id);

    for (int i = 0; i < route->cities->size; i++) {
        City *city = route->cities->data[i];

        if (print)
            sprintf(string + len, ";%s", city->name);
        len++;
        len += strlen(city->name);

        if (i < route->roads->size) {
            Road *road = route->roads->data[i];
            if (print)
                sprintf(string + len, ";%d;%d", road->length, road->builtYear);
            len++;
            len += intLen(road->length);
            len++;
            len += intLen(road->builtYear);
        }
    }
    len++;
    return len;
}

/** Zwraca wskaźnik na napis opisujący trasę.
 * Alokuje pamięć, należy potem ją zwolnić.
 * Napis jest zgodny ze specyfikacją @ref getRouteDescription.
 * @param[in] route - wskaźnik na trasę
 * @return Wskaźnik na napis lub NULL, gdy nie udao się zaalokować pamięci.
 */
char *RouteToString(Route *route) {
    char *res;
    int len;

    if (route == NULL) {
        if (!(res = malloc(sizeof(char))))
            return NULL;
        *res = (char) 0;
        return res;
    }

    len = RouteToStringHelper(route, NULL, false);

    if (!(res = malloc(len * sizeof(char))))
        return NULL;

    RouteToStringHelper(route, res, true);

    return res;
}

/** Znajduje optymalną trasę między dwoma miastami.
 * Jeżeli istnieje więcej niż jedna najlepsza trasa, zwraca pewne dwie najlepsze.
 * Optymalną, tzn. najkrótszą, zgodną ze specyfikacją @ref newRoute.
 * Znajduje ją za pomocą algorytmu Dijikstry, działa w czasie O(m log m),
 * gdzie m to liczba dróg na całej mapie.
 *
 * @param[in] cities - wskaźnik na wektor wszystkich miast
 * @param[in] source - wskaźnik na początkowe miasto
 * @param[in] target - wskaźnik na końcowe miasto
 * @param[in] avoid - wskaźnik na drogę, którą omijamy (może być NULL)
 * @param[in] avoid2 - wskaźnik na trasę, której miasta omijamy (może być NULL)
 * @return Wskaźnik na wektor zawiejący jedną lub dwie optymalne trasy lub NULL,
 * gdy nie udało się znaleźć takiej trasy lub zaalokować pamięci
 */
Vector *dijikstra(Vector *cities, City *source, City *target, Road *avoid, Route *avoid2) {
    bool ok = true;

    Heap *heap = newHeap((bool (*)(void *, void *)) cmpDistance);

    if (heap == NULL)
        return NULL;

    if (avoid2)
        for (int i = 0; i < avoid2->cities->size; i++) {
            City *c = avoid2->cities->data[i];
            if (c != source && c != target) {
                if (!(c->distance = newDistance(c, NULL)))
                    ok = false;
                if (!(c->distance2 = newDistance(c, NULL)))
                    ok = false;
            }
        }


    Distance *start = newDistance(target, NULL);
    if (!start)
        ok = false;

    else if (!insert(heap, start)) {
        deleteDistance(start);
        ok = false;
    }

    while (!empty(heap)) {
        Distance *distance = pop(heap);
        City *city = distance->city;

        if (city->distance) {
            if (city->distance2) {
                deleteDistance(distance);
                continue;
            } else {
                city->distance2 = distance;
            }
        } else
            city->distance = distance;

        for (int i = 0; i < city->roads->size; i++) {
            Road *road = city->roads->data[i];
            City *nextCity = otherCity(road, city);
            if (nextCity->distance && nextCity->distance2)
                continue;

            if (road == avoid)
                continue;

            Distance *alt = sumOfDistanceAndRoad(distance, road);
            if (alt == NULL) {
                ok = false;
                break;
            }

            if (!insert(heap, alt)) {
                ok = false;
                deleteDistance(alt);
                break;
            }
        }
    }
    deleteHeap(heap);

    Route *res = NULL, *res2 = NULL;


    if (!source->distance || !ok || !(res = nRoute(0))) {
        deleteRoute(res);
        for (int i = 0; i < cities->size; i++)
            deleteDistanceFromCity(cities->data[i]);
        return NULL;
    }

    if (cmpDistance(source->distance, source->distance2) &&
        cmpDistance(source->distance2, source->distance))
        if (!(res2 = nRoute(0))) {
            deleteRoute(res);
            for (int i = 0; i < cities->size; i++)
                deleteDistanceFromCity(cities->data[i]);
            return NULL;
        }

    City *cur = source;
    City *cur2 = source;
    while (cur != target || (res2 && cur2 != target)) {
        Road *road = cur->distance->road;
        if (cur != target)
            if (!pushBack(res->cities, cur) || !pushBack(res->roads, road) ||
                !pushBack(road->routes, res))
                ok = false;

        if (res2 && cur2 != target) {
            Road *road2 = NULL;
            if (cur == cur2)
                road2 = cur2->distance2->road;
            else
                road2 = cur2->distance->road;

            if (!pushBack(res2->cities, cur2) || !pushBack(res2->roads, road2) ||
                !pushBack(road2->routes, res2))
                ok = false;

            cur2 = otherCity(road2, cur2);
        }
        if (cur != target)
            cur = otherCity(road, cur);
    }

    if (!pushBack(res->cities, cur))
        ok = false;
    if (res2 && !pushBack(res2->cities, cur2))
        ok = false;

    for (int i = 0; i < cities->size; i++)
        deleteDistanceFromCity(cities->data[i]);

    Vector *v = NULL;
    if (!(v = newVector()))
        ok = false;
    else {
        if (!pushBack(v, res))
            ok = false;
        if (res2)
            if (!pushBack(v, res2))
                ok = false;
    }


    if (!ok) {
        deleteVector(v);
        deleteRoute(res);
        deleteRoute(res2);
        return NULL;
    }

    return v;
}

/** Znajduje optymalną trasę między dwoma miastami.
 * Jeżeli istnieje więcej niż jedna najlepsza trasa, zwraca NULL.
 * Optymalną, tzn. najkrótszą, zgodną ze specyfikacją @ref newRoute.
 * Znajduje ją za pomocą algorytmu Dijikstry, działa w czasie O(m log m),
 * gdzie m to liczba dróg na całej mapie.
 *
 * @param[in] cities - wskaźnik na wektor wszystkich miast
 * @param[in] source - wskaźnik na początkowe miasto
 * @param[in] target - wskaźnik na końcowe miasto
 * @param[in] avoid - wskaźnik na drogę, którą omijamy (może być NULL)
 * @param[in] avoid2 - wskaźnik na trasę, której miasta omijamy (może być NULL)
 * @return Wskaźnik na szuakną trasę lub NULL, gdy nie udało się jednoznacznie
 * znaleźć takiej trasy lub zaalokować pamięci
 */
Route *dijikstraOnlyOne(Vector *cities, City *source, City *target,
                        Road *avoid, Route *avoid2) {
    Vector *v = dijikstra(cities, source, target, avoid, avoid2);
    if (!v)
        return NULL;

    if (v->size == 2) {
        deleteRoute(v->data[0]);
        deleteRoute(v->data[1]);
        deleteVector(v);
        return NULL;
    }

    Route *r = v->data[0];
    deleteVector(v);
    return r;
}

/** Dodaje do trasy fragment innej trasy.
 * Dodaje @p end - @p begin - 1 miast oraz dróg,
 * począwszy od miasta na pozycji @p begin.
 * @param[in,out] route
 * @param[in] add
 * @param[in] begin
 * @param[in] end
 * @return @p false gdy nie udało się zaalokować pamięci
 * (wtedy nie modyfikuje @p route) lub @p true w przeciwnym przypadku
 */
static bool addRoute(Route *route, Route *add, int begin, int end) {
    bool ok = true;
    int cs = route->cities->size;
    int rs = route->roads->size;
    for (int i = begin; i < end; i++) {

        if (!pushBack(route->cities, add->cities->data[i]))
            ok = false;

        Road *r = add->roads->data[i];
        if (!pushBack(route->roads, r))
            ok = false;

        else if (!pushBack(r->routes, route))
            ok = false;
    }
    if (!ok) {
        while (cs != route->cities->size)
            popBack(route->cities);
        while (rs != route->roads->size) {
            Road *r = popBack(route->roads);
            deleteElementFromVector(r->routes, route);
        }
        return false;
    }
    return true;
}

/** Znajduje optymalną trasę nieprzechodzącą przez podany odcinek drogi.
 * Optymalną, tzn. najkrótszą, zgodną ze specyfikacją @ref newRoute.
 * @param[in] cities - wskaźnik na wektor wszystkich miast
 * @param[in] route - wskaźnik na starą trasę
 * @param[in] road - wskaźnik na omijaną drogę
 * @return Wskaźnik na nowa trasę lub NULL, gdy nie istnieje taki objazd lub
 * nie udało się zaalokować pamięci.
 */
Route *bypass(Vector *cities, Route *route, Road *road) {
    Route *full = nRoute(route->id);
    if (!full)
        return NULL;

    int i = 0;
    while (route->roads->data[i] != road)
        i++;

    if (!addRoute(full, route, 0, i)) {
        deleteRoute(full);
        return NULL;
    }

    City *city1 = route->cities->data[i];
    City *city2 = route->cities->data[i + 1];
    Route *alt = dijikstraOnlyOne(cities, city1, city2, road, route);

    if (!alt) {
        deleteRoute(full);
        return NULL;
    }

    if (!addRoute(full, alt, 0, alt->roads->size)) {
        deleteRoute(alt);
        deleteRoute(full);
        return NULL;
    }
    deleteRoute(alt);

    if (!addRoute(full, route, i + 1, route->roads->size)) {
        deleteRoute(full);
        return NULL;
    }

    pushBack(full->cities, route->cities->data[route->cities->size - 1]);

    return full;
}


/** Liczy długość trasy i rok budowy / remontu najstarszego odcinka trasy.
 * @param[in] route - trasa
 * @return Dystans całej trasu lub NULL, gdy nie udało się zaalokować pamięci.
 */
static Distance *getDistance(Route *route) {
    if (!route)
        return NULL;
    Distance *res = newDistance(NULL, NULL);
    if (!res)
        return NULL;

    for (int i = 0; i < route->roads->size; i++) {
        Road *r = route->roads->data[i];
        res->length += r->length;
        if (res->oldestBuiltYear > r->builtYear)
            res->oldestBuiltYear = r->builtYear;
    }
    return res;
}

/** Znajduje przedłużenie (na koniec) trasy do nowego miasta.
 * Nowe miasto może być dołączone tylko jako ostatnie miasto trasy.
 * Jeśli da się to zrobić optymalnie na więcej niż jeden sposób,
 * zwraca pewne dwie optymalne możliwości.
 * @param[in] cities - wskaźnik na wektor zawierający wszystkie miasta
 * @param[in] r - wskaźnik na trasę, którą wydłużamy
 * @param[in] city - wskaźnik na miasto, do którego wydłużamy
 * @return Wskaźnik na wektor zawiejący jedną lub dwie optymalne wydłużone trasy lub NULL,
 * gdy nie udało się znaleźć takiej trasy lub zaalokować pamięci
 */
static Vector *extendFront(Vector *cities, Route *r, City *city) {
    Vector *v = dijikstra(cities, r->cities->data[r->cities->size - 1], city, NULL, r);
    if (!v)
        return NULL;

    int s = v->size;
    for (int i = 0; i < s; i++) {
        Route *res = nRoute(r->id);
        Route *extention = v->data[i];
        if (!i)
            pushBack(v, getDistance(extention));


        if (res)
            if (!addRoute(res, r, 0, r->roads->size) ||
                !addRoute(res, extention, 0, extention->roads->size) ||
                !pushBack(res->cities, extention->cities->data[extention->cities->size - 1])) {
                deleteRoute(res);
                res = NULL;
            }

        deleteRoute(extention);
        v->data[i] = res;
    }
    return v;
}

/** Znajduje przedłużenie (na początek) trasy do nowego miasta.
 * Nowe miasto może być dołączone tylko jako pierwsze miasto trasy.
 * Jeśli da się to zrobić optymalnie na więcej niż jeden sposób,
 * zwraca pewne dwie optymalne możliwości.
 * @param[in] cities - wskaźnik na wektor zawierający wszystkie miasta
 * @param[in] r - wskaźnik na trasę, którą wydłużamy
 * @param[in] city - wskaźnik na miasto, do którego wydłużamy
 * @return Wskaźnik na wektor zawiejący jedną lub dwie optymalne wydłużone trasy lub NULL,
 * gdy nie udało się znaleźć takiej trasy lub zaalokować pamięci
 */
static Vector *extendBack(Vector *cities, Route *r, City *city) {
    Vector *v = dijikstra(cities, city, r->cities->data[0], NULL, r);
    if (!v)
        return NULL;

    int s = v->size;
    for (int i = 0; i < s; i++) {
        Route *res = nRoute(r->id);
        Route *extention = v->data[i];
        if (!i)
            pushBack(v, getDistance(extention));

        if (res)
            if (!addRoute(res, extention, 0, extention->roads->size) ||
                !addRoute(res, r, 0, r->roads->size) ||
                !pushBack(res->cities, r->cities->data[r->cities->size - 1])) {
                deleteRoute(res);
                res = NULL;
            }

        deleteRoute(extention);
        v->data[i] = res;
    }
    return v;
}

/** Znajduje wydłużenie trasy do nowego miasta.
 * Nowe miasto może być dołączone zarówno na początku jak i na końcu trasy.
 * @param[in] cities - wskaźnik na wektor zawierający wszystkie miasta
 * @param[in] r - wskaźnik na trasę, którą wydłużamy
 * @param[in] city - wskaźnik na miasto, do którego wydłużamy
 * @return Wskaźnik na nową trasę lub NULL, gdy nie udało się jej jednoznacznie wyznaczyć.
 */
Route *extendBoth(Vector *cities, Route *r, City *city) {
    Vector *fv = extendFront(cities, r, city);
    Vector *bv = extendBack(cities, r, city);

    Route *front1 = NULL, *front2 = NULL, *back1 = NULL, *back2 = NULL;
    Distance *dF = NULL;
    Distance *dB = NULL;

    if (fv) {
        dF = fv->data[fv->size - 1];
        front1 = fv->data[0];
        if (fv->size == 3)
            front2 = fv->data[1];
    }
    if (bv) {
        dB = bv->data[bv->size - 1];
        back1 = bv->data[0];
        if (bv->size == 3)
            back2 = bv->data[1];
    }

    deleteVector(fv);
    deleteVector(bv);

    if (front1 && back1 && cmpDistance(dF, dB) && cmpDistance(dB, dF)) {
        deleteDistance(dF);
        deleteDistance(dB);
        deleteRoute(front1);
        deleteRoute(front2);
        deleteRoute(back1);
        deleteRoute(back2);
        return NULL;
    }

    if (!front1 || (back1 && (cmpDistance(dF, dB)))) {
        deleteDistance(dF);
        deleteDistance(dB);

        deleteRoute(front1);
        deleteRoute(front2);

        if (back2) {
            deleteRoute(back1);
            deleteRoute(back2);
            return NULL;
        }
        deleteRoute(back2);
        return back1;
    } else {
        deleteDistance(dF);
        deleteDistance(dB);

        deleteRoute(back1);
        deleteRoute(back2);

        if (front2) {
            deleteRoute(front1);
            deleteRoute(front2);
            return NULL;
        }
        deleteRoute(front2);
        return front1;
    }

}


