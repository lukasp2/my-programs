#!/bin/bash

function print_manpage() {
	printf "NAME\n\tletfreq\n\n"
	printf "DESCRIPTION\n\tAnalyze the letter frequency of a text file\n\n"
	printf "SYNOPSIS\n\t${BWhite}letfreq.sh${NC} [${UWhite}LANGUAGE${NC}] [${UWhite}OPTION${NC}]... ${UWhite}FILE${NC}\n\n"
	printf "LANGUAGE\n\tcompares the letter frequency of the input file to that of a specified language.\n\n"
	printf "\t--en\n\t\tenglish\n"
	printf "\t--de\n\t\tgerman\n"
	printf "\t--fr\n\t\tfrench\n"
	printf "\t--es\n\t\tspanish\n"
	printf "\t--sv\n\t\tswedish\n\n"
	printf "OPTIONS\n"
	printf "\t-i\n\t\tcase-insensitive search\n"
	printf "\t-v\n\t\tverbose output\n\n"
	printf "COMMENTS\n\tThis program is dependent on bc -the arbitrary precision calculator language."
	printf "\n\tInstall by running the command apt-get install bc\n\n"
	printf "AUTHOR\n\tWritten by Lukas Pohlman.\n\n01 November 2019\n\n"
}

function print_header() {
	file_type=$(file $file | cut -d ":" -f 2 | tail -c +2)
	file_size=$(ls -alh | grep $file | cut -d " " -f 5)
	language=$(cat tables.txt | grep "$lang," | cut -d " " -f 3)


	if [[ $verbose == true ]] ; then
		printf "file\t${BYello}$file${NC}\n"
		printf "type\t$file_type\n"
		printf "size\t$file_size\n"
		printf "\n"
		printf "characters\n"
		printf "regular\t$regulars\n"
		printf "white\t$whites\n"
		printf "total\t$total\n"
	fi

	printf "\n"

	if [[ $lang_mode == true ]] ; then
		printf "\t ${BYellow}file${NC}\t\t\t $language          comparison\n"
	fi

	printf "  [%s]\t[%s]\t [%s]" "#" "char" "%"

	if [[ $lang_mode == true ]] ; then
		printf '\t|     [%s]    [%s]  \t|  [%s]\t' "char" "%" "diff,%"
	fi
	printf "\n"
}

function print_diff() {
        # get the databases letter and occurance frequency
        local line=${db_letters_arr[$iteration]}

        local letter=$(echo "$line" | cut -d " " -f 1)
        local value=$(echo "$line" | cut -d " " -f 2)

        if [[ "$value" == "" ]] ; then
                value=0
        fi

        printf "\t|\t%s      " "$letter"
        res=$(echo "$value==0" | bc -l)
        if [[ $res -eq 0 ]] ; then
                printf '%.2f' "$value"
        fi
        printf "  \t|   "

        local diff=$(echo "$percent - $value" | bc -l)

        abs $diff
        res=$(echo "$absdiff < 0.3" | bc -l)
        res2=$(echo "$absdiff < 0.6" | bc -l)
        if [[ $res -eq 1 ]] ; then
                printf "${Green}"
	elif [[ $res2 -eq 1 ]] ; then
                printf "${Yellow}"
	else
                printf "${Red}"
        fi

        local res=$(echo "$diff >= 0" | bc)
        if [[ $res -eq 1 ]] ; then
                printf "+"
        fi

        printf "%.2f ${NC}" "$diff"
}
