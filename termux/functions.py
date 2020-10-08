#!/usr/bin/env python3

import os

import colors
import helpers

links = [("youtube", "https://www.youtube.com/"),
         ("nyafilmer", "https://nyafilmer.vip/")]

def state_control(state):
    while state != 0:
        if state == 1:
            state = main_menu()
        elif state == 2:
            state = system_menu()
        elif state == 3:
            state = browser_menu()
    
def main_menu():
    prompt = ["### Welcome to the Main Menu ###", \
              "exit ssh", \
              "system control", \
              "access browser"]
    helpers.print_prompt(prompt)
    state = helpers.get_input(len(prompt) - 1)
    
    return state
    
def system_menu():
    prompt = ["## Welcome to the System Menu ##", \
              "back to main menu", \
              "volume control", \
              "system reboot", \
              "system shutdown"]
    helpers.print_prompt(prompt)
    option = helpers.get_input(len(prompt) - 1)
    
    if option == 1:
        # implement system volume control
        pass
    elif option == 2:
        print("rebooting system ...")
        os.system("sudo reboot")
    elif option == 3:
        print("shutting system down ...")
        os.system("sudo shutdown now")
        
    # return to main_menu state
    return 1
      
def browser_menu():
    prompt = ["## Welcome to the Browser Menu ##", \
              "back to main menu"]
    prompt += [website for website, link in links]
    helpers.print_prompt(prompt)
    option = helpers.get_input(len(prompt) - 1)
              
    #visit_page(links[option - 1][1])
    return 1
    
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


