#!/bin/bash

if [[ $# < 2 ]]
then
  exit 1
fi

ITER=0
for query in "$@"
do
  if [[ $ITER != 0 ]]
  then
    re='^[0-9]+$'
    if ! [[ $query =~ $re ]]
    then exit 1
    fi
 
    if [[ (( $(( $(($query < 1)) || $(($query >= 1000)) )) != 0 )) ]]
    then exit 1
    fi

    while read p; do
      if [[ ${p%%;*} == $query ]]
      then
        LENGTH=0
        LINE=$p
        LINE=${LINE#*;}
        
        while [[ $LINE != ${LINE#*;} ]]
        do
          LINE=${LINE#*;}
          WORD=${LINE%%;*}
          LINE=${LINE#*;}
          LINE=${LINE#*;}
#          echo "$WORD"
          ((LENGTH+=$WORD))
        done

        echo "$query;$LENGTH"

      fi
   done <$1

  fi

  ((++ITER))
done


exit 0
