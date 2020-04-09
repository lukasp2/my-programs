#!/bin/bash

. resources.sh
. prints.sh

declare -a db_letters_arr

parse_input $*

declare -a file_letters_arr

while IFS= read -r line ; do
	test=$(echo "$line" | cut -d ":" -f 1)
	if [[ $test == "total" ]] ; then
		total=$(echo "$line" | cut -d ":" -f 2)
	else
		letter=$(echo "$line" | cut -d " " -f 1)
		value=$(echo "$line" | cut -d " " -f 2)
		file_letters_arr+=("$letter $value")
	fi
done < <(./a.out $file)

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




