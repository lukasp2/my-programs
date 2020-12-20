#!/usr/bin/env python3

import requests
import time
import os
from selenium import webdriver
from selenium.webdriver.chrome.options import Options

class Station:
    def __init__(self, name, URL_id):
        self.name = name
        self.URL_id = URL_id

class Route:
    def __init__(self, from_station, to_station):
        self.from_station = from_station
        self.to_station = to_station

    def stringify_route(self):
        return self.from_station.name + " -> " + self.to_station.name

class Timetable:
    class TimeEntry:
        def __init__(self, orig_deparr, new_deparr, travel_time, travel_plan, switches):
            self.orig_deparr = orig_deparr
            self.new_deparr = new_deparr
            self.travel_time = travel_time
            self.travel_plan = travel_plan
            self.switches = switches
            
    def __init__(self, scraper, route):
        self.route = route
        self.timetable = []
        raw_timetable = scraper.scrape(route)
        
        for journey in raw_timetable:
            t = self.TimeEntry(journey['orig_deparr'], journey['new_deparr'], journey['travel_time'], journey['travel_plan'], journey['switches'])
            self.timetable.append(t)

    def print_timetable(self):
        if self.timetable:
            print("Timetable for route " + self.route.stringify_route())
            for journey in self.timetable:
                print(' -> '.join(journey.new_deparr), journey.travel_time, journey.travel_plan, journey.switches)
            print()
        else:
            return "No timetable was found for route " + self.route.stringify_route()
    
class SeleniumScraper:
    def __init__(self):
        options = Options()
        options.headless = True

        DRIVER_PATH = "/home/lukas/Downloads/chromedriver"
        self.driver = webdriver.Chrome(options=options, executable_path=DRIVER_PATH)

    def scrape(self, route):
        URL = "https://www.ostgotatrafiken.se/"
        URL += route.from_station.URL_id + "/" + route.to_station.URL_id
        self.driver.get(URL)

        xpath_expr = "//div[@class='departure__item accordion_list__item clearfix expandable main']"
        html_table = self.driver.find_elements_by_xpath(xpath_expr)

        timetable = []
        for table_entry in html_table:
            l = table_entry.text.split("\n")
            
            if len(l) < 4:
                continue

            l = l[:-1] if l[-1] == "Öppna" else l
            
            deviated_time = True if "-" in l[1] else False

            travel_plan = "" if len(l) - deviated_time < 4 else l[3 + deviated_time]
            travel_time = l[1 + deviated_time]
            switches = l[2 + deviated_time].split(" ")[0]
            dep_time = l[0].split("-")[0].strip()
            arr_time = l[0].split("-")[1].strip()

            new_dep_time = l[1].split("-")[0].strip() if deviated_time else dep_time
            new_arr_time = l[1].split("-")[1].strip() if deviated_time else arr_time
            
            timetable.append({'travel_plan' : travel_plan,
                              'travel_time' : travel_time,
                              'orig_deparr' : [ dep_time, arr_time ],
                              'new_deparr' : [ new_dep_time, new_arr_time ],
                              'switches' : switches,
            })

        #self.driver.close()
        return timetable


def main():
    stations = [
        Station("Matteusskolan", "matteusskolan"),
        Station("Norrköpings Resecentrum", "norrkopings-resecentrum"),
        Station("Hagaskolan", "hagaskolan-norrkoping"),
        Station("Linöpings Resecentrum", "linkopings-resecentrum"),
    ]
    
    routes_to_track = [Route(stations[0], stations[1]),
                       Route(stations[0], stations[2]),
                       Route(stations[1], stations[3]),
    ]

    scraper = SeleniumScraper()
    for route in routes_to_track:
        timetable = Timetable(scraper, route)
        timetable.print_timetable()
        
main()    

"""
requirements:
$ pip3 install selenium bs4

for selenium: 
sudo apt install google-chrome
+ chrome driver
"""

