#!/bin/bash

## prerequisites ##
# set target username, host and display
target_usr="lukas"
target_ip="192.168.2.175"
target_display=":1" # check with $who ?

# set up ssh keys if not already done
#ssh-keygen
#ssh-copy-id ${target_usr}@${target_ip}

source resources.sh

ssh ${target_usr}@${target_ip}
firefox-esr --display=${target_display}

while [ 1 ]; do
    echo "where you wanna go?"
    read input
    
    case  $input  in
        "yt")
	    link="www.youtube.com"
	    firefox-esr -new-tab=${link}
	    display_links ${link}
            ;;
        "nf")       
	    link="www.nyafilmer.wtf"
	    firefox-esr -new-tab=${link}
	    ./get_links.py ${link}
            ;;
        *)              
    esac 
done
