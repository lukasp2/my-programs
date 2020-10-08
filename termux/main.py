#!/usr/bin/env python3

import os
import functions

# set target username, host and display
target_usr = 'lukas'
target_ip = '192.168.2.175'
target_display = ':1' # check with $who ?

# connect to host and start firefox
#os.system("ssh " + target_usr + "@" + target_ip)

# start: set state to main menu
functions.state_control(1)

print("Exiting ssh session")
os.system("exit")
    
# set up ssh keys if not already done
#ssh-keygen
#ssh-copy-id ${target_usr}@${target_ip}

'''
1. better state control
2. test on ssh
'''

