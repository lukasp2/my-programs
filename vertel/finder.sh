#!/bin/bash

#read up on inheritance
#1. tree view: print children to all modules (and list each modules origin file)
#2. tree view: print children to one specific module (and list that modules origin file)
#1. finder
#2. finder res.partner

echo -e "LOOKING IN:\t\t$ODOODIR"
echo -e "SEARCHING FOR:\t\t$SEARCH"
echo -e "SEARCH_QUERY:\t\t$SEARCH_QUERY"
echo -e "RUNNING COMMAND:\tgrep -Rs inherit.\?=.\?\(\\\"\|'\)$1\(\\\"\|'\) $ODOODIR --include=*py"

grep -Rs "inherit.\?=.\?\(\\\"\|'\)$1\(\\\"\|'\)" . --include=*py



