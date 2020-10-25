#!/usr/bin/env python3

import menus

m = menus.Pick_SSH()
m.run()

# set up ssh keys if not already done
# ssh-keygen
# ssh-copy-id ${target_usr}@${target_ip}

