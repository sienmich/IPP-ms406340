#!/bin/bash

# Sprawdzenie, czy liczba parametrów jest odpowiednia
if [[ $# < 2 ]]
then
  exit 1
fi

ITER=0
for query in "$@"
do
  if [[ $ITER != 0 ]]
  then
    
    # Dla każdego zapytania sprwdzam, czy jest poprawną liczbą
    re='^[0-9]+$'
    if ! [[ $query =~ $re ]]
    then exit 1
    fi

    if [[ (( $(( $(($query < 1)) || $(($query >= 1000)) )) != 0 )) ]]
    then exit 1
    fi
    
    # Następnie szukam jego wystąpienia w pliku
    while read p; do
      if [[ ${p%%;*} == $query ]] # Jeśli wiersz p zaczyna się szukanym numerem
      then
        LENGTH=0
        LINE=$p
        LINE=${LINE#*;}
        
        while [[ $LINE != ${LINE#*;} ]]
        do
          LINE=${LINE#*;}
          WORD=${LINE%%;*} # Zmienna WORD przechodzi przez wszystkie długości dróg
          LINE=${LINE#*;}
          LINE=${LINE#*;}
          ((LENGTH+=$WORD)) # LENGTH to suma długości
        done

        echo "$query;$LENGTH"
      fi
    done <"$1"

  fi

  ((++ITER))
done


exit 0
