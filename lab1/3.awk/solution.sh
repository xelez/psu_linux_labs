#!/bin/sh

echo '1. Вывести для каждого из магазинов дни, в которые он работал.'
ls | awk -F '_' '{a[$1] = a[$1] " " $2;} END{for(i in a)print i":"a[i];}'

echo '2. Вывести в алфавитном порядке список магазинов, которые работали в определённый день.'
ls | awk -F '_' '{if ($2 == "10.10.2006") print $1;}' | sort

echo '3. Вывести для каждого из файлов отчёт вида: всего в магазине … продано товаров на сумму … рублей.'
awk 'BEGINFILE {s = 0;} {s += $2 * $3} ENDFILE { split(FILENAME, f, "_"); print f[2] ": всего в магазине " f[1] " продано товаров на сумму " s " рублей."}' *_*

echo '4. Вывести на экран количество магазинов, в которых продавался тот или иной продукт (какой именно продукт – на Ваше усмотрение)'
awk 'BEGINFILE {split(FILENAME, f, "_"); x = 0;} {if ($1 == "Мясо") x = 1;} ENDFILE{ if (x>0) k[f[1]] = 1;} END{ print "Мясо есть в " length(k) " магазинах"; }' *_*
