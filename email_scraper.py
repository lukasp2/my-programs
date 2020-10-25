#!/usr/bin/env python3

import re
import requests
from bs4 import BeautifulSoup

URL = "https://www.norrkoping.se/boende-trafik-och-miljo/boende/hyresvardar-i-norrkoping.html"
response = requests.get(URL)
soup = BeautifulSoup(response.text, 'html.parser')
emails = re.findall(r'([a-zA-Z0-9._-]+@[a-zA-Z0-9._-]+\.[a-zA-Z0-9_-]+)', response.text)
emails = list(set(emails))
emails.sort()

print(','.join(emails))



