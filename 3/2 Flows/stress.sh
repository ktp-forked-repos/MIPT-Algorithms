#!/bin/bash
for i in {0..100}
do
	echo $i
	bin/generator >input.txt
	bin/C >o1.txt
	bin/Cslow >o2.txt

	cmp --silent o1.txt o2.txt || exit
done

echo OK