#!/bin/sh

echo "1. Вывести в алфавитном порядке все слова (из всех файлов), начинающиеся с гласных."
cat *.txt | grep -i '^[aeiouy]' | sort

echo "2. Посчитать общее количество слов в файлах (по всей директории)."
cat *.txt | grep -i -c '^[a-Z]*'

echo "3. Вывести имена файлов, в которых есть слова, не имеющие гласных."
grep -ivl '[aeiouy]' *.txt

echo "4. Вывести самое последнее в алфавитном порядке слово (независимо от регистра)"
sort --reverse --ignore-case *.txt | head -n1


