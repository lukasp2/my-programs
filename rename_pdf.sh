#!/bin/bash

# rename pdf file(s) to its title

for FILE in "$@"; do
    TITLE=$(strings $FILE | grep -oE "/Title.*" | cut -d "(" -f 2 | cut -d ")" -f 1)
    NEW_NAME=$(echo $TITLE | sed 's:\s:_:g' | sed 's:/:-:g')
    echo -e "> name of PDF:\t" $FILE
    if [ "$TITLE" != "" ]; then
	echo -e "> title of PDF:\t" $TITLE
	echo -e "> new name:\t" $NEW_NAME.pdf
	mv $FILE $NEW_NAME.pdf
    else
	echo -e "> title of PDF was not found ... "
    fi
    echo ""
done
