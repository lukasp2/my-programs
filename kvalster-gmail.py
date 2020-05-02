#!/usr/bin/env python3

from bs4 import BeautifulSoup
import time
import requests
import smtplib, ssl
import sys

def notify_user(apartment_dict, apartment_ID):
    link = "https://kvalster.se/" + city + "/Uthyres/Lägenheter/" + apartment_ID
    info = '\n'.join(apartment_dict[apartment_ID]);
    msg = "Ny lägenhet hittad på kvalster!\n" + info + '\n' + link + '\n'
    print(msg)

    server = smtplib.SMTP_SSL('smtp.gmail.com', 465)
    server.login("pohlmanbot@gmail.com", "medvindsg3")
    server.sendmail("pohlmanbot@gmail.com", your_email, msg.encode('utf-8'))
    server.quit()
    
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

# search options
city = "Norrkoping" # city to search in
rooms = [1, 2]      # acceptable number of rooms
maxRent = 10000     # maximum acceptable rent
minSpace = 50       # minimum acceptable space (sq. meters)
your_email = "lukpohl3@gmail.com"

URL = "https://kvalster.se/" + city + "/Uthyres/Lägenheter?" + "Rum=" + ','.join([str(room) for room in rooms ]) + "&maxHyra=" + str(maxRent) + "&minYta=" + str(minSpace) + "&maxListad=5"

apartment_dict = get_apartments()

startTime = time.time()

search_count = 0

print("searching on Kvalster.se for apartments in " + city)
print("num rooms: " + ' or '.join([str(room) for room in rooms]))
print("max rent: ", maxRent, "kr")
print("min space:", minSpace, "kvm")
print("mailing new apartments to " + your_email + "\n")
                                
while True:
    apartments_found = get_apartments()
    
    for key in apartments_found.keys():
        if key not in apartment_dict.keys():
            apartment_dict[key] = apartments_found[key]
            notify_user(apartment_dict, key)

    output = "[ uptime: " + str(time.strftime('%H:%M:%S', time.gmtime(round(time.time() - startTime)))) + " ]" + " searching ..."
    
    sys.stdout.write("%s   \r" % (output) )

    time.sleep(30)
