/** @file
 * Interfejs klasy string - wektora przechowywującego ciąg znaków
 *
 * @author Michał Siennicki <ms406340@students.mimuw.edu.pl>
 * @copyright Michał Siennicki
 * @date 12.05.2019
 */

#ifndef __STRINGVECTOR_H__
#define __STRINGVECTOR_H__

#include <stdbool.h>
#include "vector.h"

/** @brief Dynamicznie alokowana tablica znaków.
* Na początku ma długość 0. Kiedy dodejemy na koniec nowy element,
* ale nie mieści się on w pamięci, alokuję dwa razy większą tablicę.
* Dzięki temu złożoność czasowa i pamięciowa wektora są liniowe.
*/
typedef struct String {
    int maxSize; ///< Długość zaalokowanej pamięci
    int size; ///< Długość wektora
	char *data; ///< Wskaźnik na napis
} String;

/** @brief Tworzy nowy string.
 * Tworzy nowy string, który zawiera znak końca napisu.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
String* newString();

/** @brief Usuwa string.
 * Usuwa wektor wskazywany przez @p string.
 * Nic nie robi, jeśli wskaźnik ten ma wartość NULL.
 * @param [in, out] string - wskaźnik na wektor, który usuwa
 */
void deleteString(String *string);

/** Wczytuje nowy string.
 * Alokuje pamięć pod nową strukturę i wczytuje znak po znaku stringa,
 * aż wczyta znak końca lini lub pliku.
 *
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
String* newStringRead();

int toInt(String *s);


/** Wczytuje wiersz.
 * Wczytuje cały wiersz ze standardowego wejścia.
 * Dzieli go na stringi zakończone średnikami.
 *
 * @return Wskaźnik na wektor stringów lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Vector* readLine();


#endif /* __STRINGVECTOR_H__ */
