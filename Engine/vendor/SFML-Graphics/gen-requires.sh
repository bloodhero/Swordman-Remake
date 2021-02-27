#!/bin/bash
ROOT_PATH=`pwd`

> requires.txt

for i in `echo "include"`;
do
	cd $i/SFML/Graphics/

	for j in `ls`;
	do
		grep '^#include' $j | grep -v "Graphics" | grep 'SFML' >> $ROOT_PATH/info.txt;
	done

	cd $ROOT_PATH
done

cat info.txt | sort | uniq > requires.txt
rm  -f info.txt

