#!/bin/bash

n=$(( ( RANDOM % 100 )  + 1 ))

echo "Угадайте число от 1 до 100";

while true; do
    echo 'Предположения?';
    read k;
    if ((k > n)); then
        echo 'Перебор';
    elif ((k < n)); then
        echo 'Недолёт';
    else
        echo 'Ровно в точку. Поздравляем!'
        break;
    fi
done;
