#!/usr/bin/env python3
    
def nya_filmer():
    # implement browser calls for volume control, full screen, mute, next episode / season
    pass
    
def visit_page(url):
      os.system("firefox-esr --display=:1 " + url + " &")

      print(" listing page links ..")
      links = get_links(url)

      # in either Browser or Youtube:
      # .init()_:
      # self.options = get_links(url)
      
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


