#!/bin/bash

dir_name=$1 

num_of_files=$2 

num_of_dirs=$3 

levels=$4

if [ $num_of_files -ge 0 ] && [ $num_of_dirs -ge 0 ] && [ $levels -ge 0 ]; then

bottom=999
up=127999
range=$(($up - $bottom + 1))

if [ ! -d "$dir_name" ]; then

mkdir $dir_name;

fi 

cd $dir_name;

counter=1

levelCounter=1

while [ $counter -le $num_of_dirs ] 
do

	rlength=$(cat /dev/urandom | tr -dc '1-8' | fold -w 256 | head -n 1| head --bytes 1)

	directory_names=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $rlength | head -n 1)
	echo $directory_names
	mkdir $directory_names;
	cd $directory_names;
	((counter++))


	if [ $levelCounter == $levels ]; then
		while [ $levelCounter -ge 1 ]
		do
			cd ..;
			((levelCounter--))
		done
	fi
	((levelCounter++))
done

counter=1

while [ $levelCounter -ge 2 ]
do
	cd ..;
        ((levelCounter--))
done
listCounter=1
levelCounter=1
listLen=$(find . -mindepth 1 -maxdepth 1 -type d | wc -w)

while [ $counter -le $num_of_files ]
do

	TlistLen=$(find . -mindepth 1 -maxdepth 1 -type d | wc -w)
	if [ $TlistLen == 1 ]; then
		
		number=$RANDOM
                let "number %= $range";
                number=$(($number + $bottom))
                flength=$number

                fileText=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $flength | head -n 1)
                echo $fileText > f$counter
                ((counter++))

		cd $(ls -d -1 */ |sed -n '1p')

		((levelCounter++))
	else
		if [ $TlistLen != 0 ]; then
			if [ $listCounter == 1 ]; then

				number=$RANDOM
                        	let "number %= $range";
                        	number=$(($number + $bottom))
                        	flength=$number
	                	fileText=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $flength | head -n 1)
        	        	echo $fileText > f$counter
               			((counter++))
			fi
			cd $(ls -d -1 */ |sed -n $listCounter'p')
			levelCounter=2
			if [ $listCounter == $listLen ]; then
				listCounter=0
			fi
			((listCounter++))

		else	
			number=$RANDOM
			let "number %= $range";
			number=$(($number + $bottom))
			flength=$number

                        fileText=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $flength | head -n 1)
                        echo $fileText > f$counter
                        ((counter++))
			while [ $levelCounter != 1 ]
			do
				cd ..;
				((levelCounter--))
			done
		fi
	fi

done

else

echo wrong numbers
fi
