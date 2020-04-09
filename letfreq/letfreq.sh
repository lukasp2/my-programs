#!/bin/bash

. resources.sh
. prints.sh

declare -a db_letters_arr

#searches for flags, loads tables et.c.
parse_input $*

total=0
whites=0
regulars=0
escape="[\'\"@*]"
regular="[A-Za-z0-9!#$%^&()\]"

declare -A file_letters

while read -r -n1 char; do
	if [[ "$char" =~ $escape ]] ; then
		char='\'"${char}"
	fi
	if [[ "$char" =~ $regular ]] ; then

		if [[ $case_insensitive == true ]] ; then
                        char=$(echo "$char" | tr '[:upper:]' '[:lower:]')
                fi

		((file_letters[$char]++))
		((regulars++))
	else
		((whites++))
	fi
	((total++))
done < $file

if [ $total -eq 0 ] || [ ${#file_letters[@]} -eq 0 ] ; then
	printf "Nothing to analyze, exiting ...\n"
	exit 1
fi

declare -a file_letters_arr

to_array

print_header

# used to match file_letters and db_letters
iteration=0
for line in "${file_letters_arr[@]}" ; do
  	letter=$(echo "$line" | cut -d " " -f 1)
    	count=$(echo "$line" | cut -d " " -f 2)

	printf '%5d\t  %s\t' "$count" "$letter"

	percent=$(bc <<< "scale=2; $count * 100 / $total")
	printf '%.2f' "$percent"

	if [[ $lang_mode == true ]] ; then
		print_diff
		((iteration++))
	fi

	# graph
	printf "\t"

	num=$(echo "($percent * 3) / 1" | bc)
	for ((i=0;i<$num;i++)) ; do
		printf "#"
	done

	printf '\n'
done

printf "\n"




