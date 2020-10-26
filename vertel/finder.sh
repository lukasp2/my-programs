#!/bin/bash

#read up on inheritance
#1. tree view: print children to all modules (and list each modules origin file)
#2. tree view: print children to one specific module (and list that modules origin file)
#1. finder
#2. finder res.partner

source ../colors.sh

ODOODIR="/usr/share"
SEARCH="$1"

if (( $# != 1 )); then
    echo "usage: finder [OPTIONS] [class name] ..."
    # options:
    #   -f: (DEFAULT) find class (eg. res.partner)
    #   -c: list children to class (all that inherits res.partner)
    #   LATER: -p: list parent to class ()
    exit
fi

echo -e "LOOKING IN\t\t$ODOODIR"
echo -e "SEARCHING FOR\t\t$SEARCH"
echo -e "RUNNING COMMAND\t\tgrep -Rns inherit.\?=.\?\(\\\"\|'\)$1\(\\\"\|'\) $ODOODIR --include=*py"

# -fc:
echo -e "\n${Yellow}ORIGIN${NC}"
grep -Rns "_name.\?=.\?\(\\\"\|'\)$SEARCH\(\\\"\|'\)" /usr/share --include=*py
echo -e "\n${Yellow}INHERITANCES${NC}"
grep -Rns "_inherit.\?=.\?\(\\\"\|'\)$SEARCH\(\\\"\|'\)" /usr/share --include=*py

# -c:
#grep -Rs "_inherit.\?=.\?\(\\\"\|'\)$1\(\\\"\|'\)" . --include=*py


