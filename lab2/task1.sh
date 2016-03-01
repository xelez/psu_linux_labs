#!/bin/bash

echo -n 'Prime check. Please, enter number > '
read n

sqrt_n=`echo "sqrt($n)" | bc `

for ((i=2; $i<=$sqrt_n; i+=1)); do
    if (($n % $i == 0)); then
        echo 'Not prime';
        exit 1;
    fi;
done;

echo 'Is prime';
exit 0;
