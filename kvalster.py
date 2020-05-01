#!/usr/bin/env python3

import time
import requests
from bs4 import BeautifulSoup

import fbchat 
from getpass import getpass 

# search options
city = "Norrkoping"
rooms = [1, 2]
maxRent = 10000
minSpace = 50

# facebook (see general settings on facebook)
username = "patte.hatt"
real_name = "Lukas Pohlman"
client = fbchat.Client(username, getpass())
# if you have two factor authentication enabled (2FA), you need to add a key to your fb account

# query URL
URL = "https://kvalster.se/" + city + "/Uthyres/Lägenheter?" + "Rum=" + ','.join([str(room) for room in rooms ]) + "&maxHyra=" + str(maxRent) + "&minYta=" + str(minSpace) + "&maxListad=5"

# sends notification to user
def notify_user(apartment_dict, apartment_ID):
    link = "https://kvalster.se/" + city + "/Uthyres/Lägenheter/" + apartment_ID
    info = '\n'.join(apartment_dict[apartment_ID]);
    msg = "New apartment added to kvalster!\n" + info + '\n' + link + '\n'
    print(msg)

    friend = client.searchForUsers(real_name)[0]
    sent = client.send(fbchat.models.Message(msg), friend.uid)
    
    if sent:
        print("Message sent successfully!")
        
# returns all apartments found
def get_apartments():
    apartment_dict = {}

    html_doc = requests.get(URL)
    soup = BeautifulSoup(html_doc.text, 'html.parser')
    results = soup.find_all('a')

    for result in results[2:-4]:
        line = str(result).split("/Lägenheter/")[1]
        line = line.replace("\">", " ").replace("</a>", "").replace("<br/>", " ")

        apartment_ID = line.split(" ")[0]
        apartment_info = ' '.join(line.split(" ")[1:])

        if (not apartment_ID in apartment_dict):
            apartment_dict[apartment_ID] = []
            
        apartment_dict[apartment_ID].append(apartment_info)

    return apartment_dict

# log all apartments found initially { apartment_ID : apartment_info }
apartment_dict = get_apartments()
# apartment_dict = {}

while True:
    apartments_found = get_apartments()
    
    # find apartments stored in apartments_found that is not stored in apartments_dict
    for key in apartments_found.keys():
        if key not in apartment_dict.keys():
            apartment_dict[key] = apartments_found[key]
            notify_user(apartment_dict, key)

    print("searching ...")
            
    time.sleep(30)
