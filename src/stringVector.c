/** @file
 * Implementacja klasy string - wektora przechowywującego ciąg znaków
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 12.05.2019
 */

#include <stdio.h>
#include <stdlib.h>
#include "stringVector.h"
#include "vector.h"

/** @brief Tworzy nowy string.
 * Tworzy nowy string, który zawiera znak końca napisu.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
String *newString() {
    String *string;
    if (!(string = malloc(sizeof(String))))
        return NULL;

    string->maxSize = 1;
    string->size = 1;
    string->data = NULL;

    if (!(string->data = malloc(sizeof(char)))) {
        free(string);
        return NULL;
    }
    string->data[0] = 0;

    return string;
}

/** @brief Usuwa string.
 * Usuwa wektor wskazywany przez @p string.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param [in, out] string - wskaźnik na wektor, który usuwa
 */
void deleteString(String *string) {
    if (!string)
        return;
    free(string->data);
    free(string);
}

/** Alokuje nową pamięć dla wektora.
 * Nie zmienia jego rozmiaru.
 *
 * @param[in,out] string - wskaźnik na wektor
 * @param[in] newMaxSize - nowy rozmiar alokowanej pamięci
 * @return @p false gdy nie udało się zaalkować pamięci,
 * @p true w przeciwnym przypadku
 */
static bool reMaxSize(String *string, int newMaxSize) {
    char *tmp = realloc(string->data, newMaxSize * sizeof(char));
    if (tmp == NULL)
        return false;

    string->maxSize = newMaxSize;
    string->data = tmp;
    return true;
}


/** Alokuje dwa razy więcej pamięci na wektor.
 * @param string - wskaźnik na wektor
 * @return @p false gdy nie udało się zaalkować pamięci,
 * @p true w przeciwnym przypadku
 */
static bool extend(String *string) {
    if (string->maxSize == 0)
        return reMaxSize(string, 1);
    else
        return reMaxSize(string, string->maxSize * 2);
}


/** @brief Dodaje nowy znak na koniec wektora.
 * @param[in,out] string - wskaźnik na wektor
 * @param[in] c - nowy znak
 * @return Wartość @p true, jeśli operacja się powiodła.
 * Wartość @p false, jeśli nie udało się zaalokować pamięci.
 */
static bool pushBackChar(String *string, char c) {
    if (string->size == string->maxSize)
        if (!extend(string))
            return false;

    string->data[string->size - 1] = c;
    string->data[string->size++] = 0;
    return true;
}

/** Wczytuje nowy string.
 * Alokuje pamięć pod nową strukturę i wczytuje znak po znaku stringa,
 * aż wczyta średnik, znak końca lini lub pliku.
 *
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
String *newStringRead() {
    String *s = newString();
    if (!s)
        return NULL;
    char c = 'a';
    while (c != EOF && c != ';' && c != '\n') {

        if (scanf("%c", &c) == EOF)
            c = EOF;
        if (!pushBackChar(s, c)) {
            deleteString(s);
            return NULL;
        }
    }
    return s;
}

/** Zwraca liczbę na podstawie ciągu znaków.
 * Dopuszcza zera wiodące oraz liczby ujemne.
 * W przypadku błędu konwersji - za długa liczba lub znaki nie będące cyframi
 * zwraca 0.
 *
 * @param[in] s - wskaźnik na stringa
 * @return wartość liczbowa napisu lub @p 0 w przypadku nieudanej konwersji
 */
int toInt(String *s) {
    int res = 0;
    int i = 0;
    if (s->data[0] == '-')
        i++;
    while (s->data[i]) {
        if (s->data[i] < '0' || s->data[i] > '9')
            return 0;
        res *= 10;
        res += s->data[i] - '0';
        if (res < 0)
            return 0;
        i++;
    }
    if (s->data[0] == '-')
        res = -res;

    return res;
}

/** Zwraca tablicę znaków odpowiadającą stringowi.
 *
 * @param[in] s - wskaźnik na stringa
 * @return wskaźnik na tablicę znaków
 */
char *toCharArray(String *s) {
    return s->data;
}

/** Wczytuje wiersz.
 * Wczytuje cały wiersz ze standardowego wejścia.
 * Dzieli go na stringi zakończone średnikami.
 *
 * @return Wskaźnik na wektor stringów lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Vector *readLine() {
    Vector *v = newVector();
    if (!v)
        return NULL;

    while (true) {
        String *s = newStringRead();
        if (!s || !pushBack(v, s)) {
            for (int i = 0; i < v->size; i++)
                deleteString(v->data[i]);
            deleteVector(v);
            return NULL;
        }

        s->data[s->size - 1] = s->data[s->size - 2];
        s->data[s->size - 2] = 0;
        if (s->data[s->size - 1] != ';')
            break;
    }

    return v;
}