#!/usr/bin/env python3

import requests
from bs4 import BeautifulSoup

url = input()
r = requests.get(url)
html_content = r.text
soup = BeautifulSoup(html_content, 'lxml')

links = soup.find_all('a')

for link in links:
    url = link.get('href')
    print(url)
    

