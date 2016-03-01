#!/bin/sh

echo '1. Вывести на экран все строчки файла, кроме строк, в котором есть error(или ERROR).'
cat some.log | sed  '/\b\(error\|ERROR\)\b/d'

echo '2. После каждой строчки со словом error (или ERROR) вставить строчку Achtung!!!'
cat some.log | sed  '/\b\(error\|ERROR\)\b/ a\Achtung!!!'

echo '3. Вместо строчки, содержащей error (или ERROR)  вставить строчку Achtung!!!(исходная строка, содержащая error - удаляется)'
cat some.log | sed  's/.*\b\(error\|ERROR\)\b.*/Achtung!!!/'

echo '4. Зашифровать все строчки, содержащие error (или ERROR), так, чтобы символу a соответствовал символ z (a->z), b->y, c->x и так далее.'
prog='y/abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/zyxwvutsrqponmlkjihgfedcbaZYXWVUTSRQPONMLKJIHGFEDCBA/'
cat some.log | sed $prog > some.crypted
cat some.crypted | sed $prog > tmp.log
diff some.log tmp.log && echo 'Ok' || echo 'Fail'
