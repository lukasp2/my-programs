#!/usr/bin/env python3

import os
import colors

links = [("youtube", "https://www.youtube.com/"),
         ("nyafilmer", "https://nyafilmer.vip/")]

def system_menu():
    print(colors.Green + "### Welcome to the System Menu ###" + colors.NC)
    print("0. exit")
    print("1. volume control")
    print("2. access browser")
    print("3. system reboot")
    print("4. system shutdown")
    print("")
    ## full screen, start, pause, next episode, ...
    
    while True:
        option = int(input())
        if option == 0:
            print("Exiting ssh session")
            os.system("exit")
            exit()
        elif option == 1:
            # implement system volume control
            pass
        elif option == 2:
            browser_menu()
        elif option == 3:
            print("rebooting system ...")
            os.system("sudo reboot")
        elif option == 3:
            print("shutting down system ...")
            os.system("sudo shutdown now")
        else:
            print("invalid option, try again ...")
      
def browser_menu():
      print(colors.Green + "## Welcome to the Browser Menu ##" + colors.NC)
      print("0. go back")
      for i in range(len(links)):
            print(str(i + 1) + ". " + links[i][0])
      print("")
      
      while True:
            option = int(input())
            if option == 0:
                system_menu()
            elif 1 <= option <= len(links):
                visit_page(links[option - 1][1])
            else:
                print("invalid option, try again ...")
                
def nya_filmer():
    # implement browser calls for volume control, full screen, mute, next episode / season
    pass
    
def visit_page(url):
      os.system("firefox-esr --display=:1 " + url + " &")

      print(" listing page links ..")
      links = get_links(url)

      for i in range(len(links)):
            print(i + 1, links[i])

      print("choose a link ...")

      while True:
            option = int(input())
            
            if option == 0:
                system_menu()
            elif 1 <= option <= len(links):
                  visit_page(links[option - 1])
            else:
                  print("invalid option, try again ...")
                  
def get_links(url):
      import requests
      from bs4 import BeautifulSoup

      r = requests.get(url)
      html_content = r.text
      soup = BeautifulSoup(html_content, 'lxml')

      links = soup.find_all('a')

      ret = []
      for link in links:
            url = link.get('href')
            ret.append(url)
            
      return ret


