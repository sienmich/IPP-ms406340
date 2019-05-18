/** @file
 * Program, który korzystając z modułu map.h udostępnia operacja na mapie.
 * Program czyta dane ze standardowego wejścia, wyniki wypisuje na standardowe wyjście,
 * a informacje o błędach na standardowe wyjście diagnostyczne.
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 15.05.2019
 */



#include "map.h"
#include "stringVector.h"
#include "vector.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** Przetwarza zapytanie o mapę.
 * Dokładna specyfikacja zapytań znajduje się w
 * [treści zadania](https://moodle.mimuw.edu.pl/mod/assign/view.php?id=13381).
 *
 * @param[in,out] m - wskaźnik na mapę
 * @param[in] line - wskaźnik na wektor stringów
 * @return @p false gdy wystąpił błąd, @p true w przeciwnym wypadku.
 */
static bool processQuery(Map *m, Vector *line) {
    String *first = line->data[0];

    if (first->data[0] == '#')
        return true;

    if (!strcmp(first->data, "") && line->size == 1)
        return true;

    if (!strcmp(first->data, "addRoad") && line->size == 5) {
        int length = toInt(line->data[3]);
        int builtYear = toInt(line->data[4]);
        char *city1 = toCharArray(line->data[1]);
        char *city2 = toCharArray(line->data[2]);

        return addRoad(m, city1, city2, length, builtYear);
    }

    if (!strcmp(first->data, "repairRoad") && line->size == 4) {
        int repairYear = toInt(line->data[3]);
        char *city1 = toCharArray(line->data[1]);
        char *city2 = toCharArray(line->data[2]);

        return repairRoad(m, city1, city2, repairYear);
    }

    if (!strcmp(first->data, "getRouteDescription") && line->size == 2) {
        String *second = line->data[1];
        int routeId = toInt(second);

        if (routeId <= 0 && strcmp(toCharArray(second), "0"))
            return false;
        char *res = (char *) getRouteDescription(m, routeId);
        if (!res)
            return false;
        else {
            printf("%s\n", res);
            free(res);
            return true;
        }
    }

    return newRouteFromDescription(m, line);
}

/** Wczytuje i przetwarza zapytania o mapę.
 * Wczytuje dane ze standardowego wejścia, wyniki wypisuje na standardowe wyjście,
 * a informacje o błędach na standardowe wyjście diagnostyczne.
 * Dokładna specyfikacja zapytań znajduje się w
 * [treści zadania](https://moodle.mimuw.edu.pl/mod/assign/view.php?id=13381).
 *
 * @return @p 0
 */
int main() {

    Map *m = newMap();
    if (!m)
        return 0;

    for (int lineNr = 1; true; lineNr++) {
        Vector *line = readLine();
        if (!line) {
            deleteMap(m);
            return 0;
        }

        if (!processQuery(m, line))
            fprintf(stderr, "ERROR %d\n", lineNr);

        String *last = line->data[line->size - 1];
        bool kon = last->data[last->size - 1] == EOF;

        for (int i = 0; i < line->size; i++)
            deleteString(line->data[i]);
        deleteVector(line);

        if (kon)
            break;
    }

    deleteMap(m);
    return 0;
}
