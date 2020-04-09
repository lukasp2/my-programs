#!/bin/bash

function load_language() {
	lang_mode=true
        lang=$(echo $1 | cut -d "-" -f 3)

        table=$(cat ./tables.txt | grep -A 27 "$lang:" | cut -d ":" -f 2 | tail -c +2 )

	if [[ $table == "" ]] ; then
		echo "language not found"
		exit 1
	fi

        declare -A db_letters
        while read -r line ; do
                letter=$(echo "$line" | cut -d " " -f 1)
                value=$(echo "$line" | cut -d " " -f 2)
                db_letters[$letter]=$value
        done < <(printf '%s' "$table")

        sort_db_letters
}

function parse_input() {
	input=$@

	for a in $@ ; do
		if [[ $a == "--help" ]] ; then
			print_manpage ; exit -1

		elif [[ $a =~ "--" ]] ; then
			load_language $a

		elif [[ $a == "-i" ]] ; then
			case_insensitive=true

		elif [[ $a == "-v" ]] ; then
			verbose=true

		else
			if [[ -f "$a" ]] ; then
				file=$a
			else
				echo "file '$a' could not be found"
				exit 1
			fi
		fi
	done
}

function print_sorted_db_letters() {
        for key in "${sorted[@]}"; do
                printf '%s %.3f\n' "$key" "${db_letters[$key]}"
        done | sort -rn -k2
}

function print_db_letters() {
        for key in "${!db_letters[@]}"; do
            echo "$key ${db_letters[$key]}"
        done | sort -rn -k2
}

function sort_db_letters() {

        mapfile -d '' sorted < <(printf '%.3f\0' "${db_letters[@]}" | sort -z)

        sorted=()
        while IFS= read -r row; do
                key=$(echo $row | cut -d " " -f 1)
                sorted+=( "$key" )
        done < <(print_sorted_db_letters)

	while IFS= read -r line ; do
		letter=$(echo "$line" | cut -d " " -f 1)
		value=$(echo "$line" | cut -d " " -f 2)
		db_letters_arr+=("$letter $value")
	done < <(print_db_letters)
}

function abs() {
	number=$1

	res=$(echo "$number < 0" | bc -l)

	if [[ $res -eq 1 ]] ; then
		number=$(echo "$number * -1" | bc -l)
	fi

	absdiff=$number
}
# Reset
NC='\033[0m'       # Text Reset

# Regular Colors
Black='\033[0;30m'        # Black
Red='\033[0;31m'          # Red
Green='\033[0;32m'        # Green
Yellow='\033[0;33m'       # Yellow
Blue='\033[0;34m'         # Blue
Purple='\033[0;35m'       # Purple
Cyan='\033[0;36m'         # Cyan
White='\033[0;37m'        # White

# Bold
BBlack='\033[1;30m'       # Black
BRed='\033[1;31m'         # Red
BGreen='\033[1;32m'       # Green
BYellow='\033[1;33m'      # Yellow
BBlue='\033[1;34m'        # Blue
BPurple='\033[1;35m'      # Purple
BCyan='\033[1;36m'        # Cyan
BWhite='\033[1;37m'       # White

# Underline
UBlack='\033[4;30m'       # Black
URed='\033[4;31m'         # Red
UGreen='\033[4;32m'       # Green
UYellow='\033[4;33m'      # Yellow
UBlue='\033[4;34m'        # Blue
UPurple='\033[4;35m'      # Purple
UCyan='\033[4;36m'        # Cyan
UWhite='\033[4;37m'       # White

# Background
On_Black='\033[40m'       # Black
On_Red='\033[41m'         # Red
On_Green='\033[42m'       # Green
On_Yellow='\033[43m'      # Yellow
On_Blue='\033[44m'        # Blue
On_Purple='\033[45m'      # Purple
On_Cyan='\033[46m'        # Cyan
On_White='\033[47m'       # White

# High Intensity
IBlack='\033[0;90m'       # Black
IRed='\033[0;91m'         # Red
IGreen='\033[0;92m'       # Green
IYellow='\033[0;93m'      # Yellow
IBlue='\033[0;94m'        # Blue
IPurple='\033[0;95m'      # Purple
ICyan='\033[0;96m'        # Cyan
IWhite='\033[0;97m'       # White

# Bold High Intensity
BIBlack='\033[1;90m'      # Black
BIRed='\033[1;91m'        # Red
BIGreen='\033[1;92m'      # Green
BIYellow='\033[1;93m'     # Yellow
BIBlue='\033[1;94m'       # Blue
BIPurple='\033[1;95m'     # Purple
BICyan='\033[1;96m'       # Cyan
BIWhite='\033[1;97m'      # White

# High Intensity backgrounds
On_IBlack='\033[0;100m'   # Black
On_IRed='\033[0;101m'     # Red
On_IGreen='\033[0;102m'   # Green
On_IYellow='\033[0;103m'  # Yellow
On_IBlue='\033[0;104m'    # Blue
On_IPurple='\033[0;105m'  # Purple
On_ICyan='\033[0;106m'    # Cyan
On_IWhite='\033[0;107m'   # White
