#!/bin/bash

logFiles=$(cat -)
clients=$(echo "$logFiles" | cut -d ' ' -f 2)
clients=$(echo "$clients" | sort)
clients=$(echo "$clients" | uniq)

number_of_clients=$(echo "$clients" | wc -l)
echo number of clients: $number_of_clients

echo clients: $(echo $clients)

set -- $clients
min=$1
for i in $clients;
do
	if [ $i -le $min ]; then
		min=$i
	fi
done

max=$1
for i in $clients;
do
        if [ $i -ge $max ]; then
                max=$i
        fi
done

echo min id: $min
echo max id: $max

bytes=$(echo "$logFiles" | cut -d ' ' -f 1,4)
bytes_read=$(echo "$bytes" | grep "read" )
bytes_read=$(echo "$bytes_read"	| cut -d ' ' -f 2 )
sum=0

for i in $bytes_read;
do
	sum=$(($sum + $i))
done
echo received bytes: $sum

bytes_write=$(echo "$bytes" | grep "write" )
bytes_write=$(echo "$bytes_write" | cut -d ' ' -f 2)

sum=0
for i in $bytes_write;
do
        sum=$(($sum + $i))
done
echo sent bytes: $sum

files=$(echo "$logFiles" | cut -d ' ' -f 1,3)
files_read=$(echo "$files" | grep "read")
files_read=$(echo "$files_read" | cut -d ' ' -f 2 )
count=0
for i in $files_read;
do
        count=$(($count + 1))
done
echo files received: $count

files=$(echo "$logFiles" | cut -d ' ' -f 1,3)
files_write=$(echo "$files" | grep "write")
files_write=$(echo "$files_write" | cut -d ' ' -f 2 )
count=0
for i in $files_write;
do
        count=$(($count + 1))
done
echo files sent: $count



