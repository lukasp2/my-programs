#!/usr/bin/env python3

import os
import socket
import requests
from bs4 import BeautifulSoup

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
            
            self.handle_input(option)
            
class Pick_SSH(Menu):
    def __init__(self):
        header = "## Pick SSH Menu ##"
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
        header = "## Main Menu ##"
        options = ["exit", \
                   "system control", \
                   "access browser"]

        Menu.__init__(self, header, options)

    def handle_input(self, option):
        if option == 1:
            m = System_Menu()
            m.run()
        elif option == 2:
            m = Browser_Menu()
            m.run()
        else:
            print("An error occured ...")

class System_Menu(Menu):
    def __init__(self):
        header = "## System Menu ##"
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
            print()
        else:
            print("An error occured ...")

links = {"youtube" : "https://www.youtube.com/",
         "nyafilmer" : "https://nyafilmer.vip/"}

class Browser_Menu(Menu):    
    def __init__(self):
        header = "## Browser Menu ##"
        options = ["back to Main Menu"]
        options += links.keys()
        
        super().__init__(header, options)

    def handle_input(self, option):
        if option == 1:
            m = YouTube()
            m.run()
        elif option == 2:
            m = NyaFilmer()
            m.run()
        else:
            print("An error occured ...")

class Webpage_Viewer():
    def __init__(self, webpage):
        self.webpage = webpage
        
    def get_links(self, url):
        html_content = requests.get(url).text
        soup = BeautifulSoup(html_content, 'lxml')
        hyperlinks = [ link.get('href') for link in soup.find_all('a') ]
        hyperlinks = self.parse_links(hyperlinks)
        return hyperlinks

    def make_valid_link(self, link):
        if link.find(links[self.webpage]) == -1:
            link = links[self.webpage] + link
        return link
    
    def parse_links(self, hyperlinks):
        hyperlinks = sorted(list(set(filter(None, hyperlinks))))
        hyperlinks = [ self.make_valid_link(link) for link in hyperlinks ]
        return hyperlinks
    
class YouTube(Menu):
    def __init__(self):
        header = "## YouTube ##"
        self.webpage_viewer = Webpage_Viewer('youtube')
        self.options = ["back to Browser Menu"]
        self.options += self.webpage_viewer.get_links(links['youtube'])
        super().__init__(header, self.options)

    def handle_input(self, option):        
        os.popen("firefox-esr --display=:1 " + options[option - 1])

class NyaFilmer(Menu):
    def __init__(self):
        header = "## NyaFilmer ##"
        self.webpage_viewer = Webpage_Viewer('nyafilmer')
        self.options = ["back to Browser Menu"]
        self.options += self.webpage_viewer.get_links(links['nyafilmer'])
        super().__init__(header, self.options)
    
    def handle_input(self, option):
        os.popen("firefox-esr --display=:1 " + self.options[option - 1])

