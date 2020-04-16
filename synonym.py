#!/usr/bin/env python3

# lists synonyms for word given as input argument

import sys
import requests
from bs4 import BeautifulSoup

if len(sys.argv) != 2:
    print('invalid number of arguments')
    exit()

word = str(sys.argv[1])

response = requests.get('https://www.synonymer.se/sv-syn/' + word)
soup = BeautifulSoup(response.text, 'html.parser')

results = soup.find_all('ol')

if not results:
    print('inga synonymer hittades till ' + word)
    exit()

print(word)
text = results[0].get_text().lstrip().rstrip().replace(";", ";\n\t")
print('\t', text, '\n')
    
'''
for r in results:
    text = r.get_text().lstrip().rstrip()
    if text:
        print('>', text, '\n')
'''
