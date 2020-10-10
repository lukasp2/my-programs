#!/usr/bin/env python3

import os
import socket
import colors

class Menu:
    def __init__(self, header, options):
        self.header = header
        self.options = options
    
    def print_menu_prompt(self):
        print(colors.Green + self.header + colors.NC)
        for i in range(len(self.options)):
            print(str(i) + ". " + self.options[i])

    def get_menu_input(self):
        print(">>> ", end='')
        opt = int(input())
    
        while opt < 0 or opt >= len(self.options):
            print("Invalid input, try again ...")
            print(">>> ", end='')
            opt = int(input())

        print()
        return opt

    # abstract class, force override:     
    def handle_input(self, option):
        pass

    def run(self):
        while True:
            self.print_menu_prompt()
            option = self.get_menu_input()

            if option == 0:
                break
            
            m = self.handle_input(option)
            # id type(self) == type(m) then ???
            m.run()
            
class Pick_SSH(Menu):
    def __init__(self):
        header = "## Welcome to the Pick SSH Menu ##"
        options = ["exit"]
        self.lookup_nw_hosts()
        for host in self.hosts:
            options.append(host['hostname'] + " " + host['ip'])
            
        super().__init__(header, options)
        
    def lookup_nw_hosts(self):
        local_ip = socket.gethostbyname(socket.gethostname())
        x = os.popen("ip add").read()
        x = x[x.find(local_ip):]
        netmask = x[x.find("/") : x.find("/") + 3]
        nmap_output = os.popen("nmap -sn " + local_ip + netmask).read().split('\n')
        
        self.hosts = []
        for row in nmap_output:
            if row[:20] == "Nmap scan report for":
                s = row[21:].split()
                hostname = s[0]
                ip = s[1][1:-1]
                self.hosts.append({'hostname' : hostname, 'ip' : ip})
        
    def handle_input(self, option):
        os.popen("ssh " + self.hosts[option - 1]['hostname'] + "@" + self.hosts[option - 1]['ip'])
        
class Main_Menu(Menu):
    def __init__(self):
        header = "## Welcome to the Main Menu ##"
        options = ["exit", \
                   "system control", \
                   "access browser"]

        Menu.__init__(self, header, options)

    def handle_input(self, option):
        if option == 1:
            return System_Menu()
        elif option == 2:
            return Browser_Menu()
        else:
            print("An error occured ...")
            return self

class System_Menu(Menu):
    def __init__(self):
        header = "## Welcome to the System Menu ##"
        options = ["back to main menu", \
                  "volume control", \
                  "system reboot", \
                   "system shutdown", \
                   "system info"]
        Menu.__init__(self, header, options)

    def handle_input(self, option):
        if option == 1:
            # implement system volume control
            # return Volume_Menu() ??
            pass
        elif option == 2:
            print("rebooting system ...")
            os.system("sudo reboot")
        elif option == 3:
            print("shutting system down ...")
            os.system("sudo shutdown now")
        elif option == 4:
            print("System info")
            x = os.uname()
            print(*x, sep='\n')
            input("Press any key to continue ...")
        else:
            print("An error occured ...")
            
        return self

class Browser_Menu(Menu):
    # TODO: get tty/target display from $who
    
    def __init__(self):
        header = "## Welcome to the Browser Menu ##"
        self.links = [("youtube", "https://www.youtube.com/"),
                      ("nyafilmer", "https://nyafilmer.vip/")]

        options = ["back to main menu"]
        for link in self.links:
            options.append(link[0])
        
        super().__init__(header, options)

    def handle_input(self, option):
        if option == 1:
            print("going to youtube ...")
            #return Browser_Youtube() ??
            #or: firefox-esr ....
        elif option == 2:
            print("going to nyafilmer ...")
            #return Browser_NyaFilmer() ??
            #or: firefox-esr ....
        else:
            print("An error occured ...")
            return Main_Menu()

# class Webpage_Viewer(Menu):
# > class YouTube(Webpage_Viewer):


