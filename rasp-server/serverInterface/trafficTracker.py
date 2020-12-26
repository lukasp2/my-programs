import requests
import time
import os
 
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

class Station:
    def __init__(self, name, URL_id):
        self.name = name
        self.URL_id = URL_id

class Route:
    def __init__(self, from_station, to_station):
        self.from_station = from_station
        self.to_station = to_station

    def stringify(self):
        return self.from_station.name + " -> " + self.to_station.name
    
class Journey:
    def __init__(self,
                 orig_deparr,
                 new_deparr,
                 travel_time,
                 travel_plan,
                 switches,
                 deviated_time):
        self.journey = {}
        self.journey['orig_deparr'] = orig_deparr
        self.journey['new_deparr'] = new_deparr
        self.journey['travel_time'] = travel_time
        self.journey['travel_plan'] = travel_plan
        self.journey['switches'] = switches
        self.journey['deviated_time'] = deviated_time

    def _print(self):
        print(' -> '.join(self.journey['new_deparr']),
              self.journey['travel_time'],
              self.journey['travel_plan'],
              self.journey['switches'],
              '*' if self.journey['deviated_time'] else '')

    def stringify(self):
        dev_star = '*' if self.journey['deviated_time'] else ''
        s = ' -> '.join(self.journey['new_deparr']) + " " + \
            str(self.journey['travel_time']) + " " + \
            self.journey['travel_plan'] + " " + \
            str(self.journey['switches']) + " " + \
            dev_star
        return s

    def stringify_minimal(self):
        dev_star = '*' if self.journey['deviated_time'] else ''
        s = self.journey['new_deparr'][0] + " " + \
            "(" + str(self.journey['travel_time']) + ") " + \
            self.journey['travel_plan'] + " " + \
            dev_star
        return s
    
class Timetable:
    def __init__(self, scraper, route):
        self.route = route
        self.timetable = scraper.scrape(route)

    def _print(self):
        print("Timetable for route " + self.route.stringify())
        if self.timetable:
            for journey in self.timetable:
                journey._print()
            print()
        else:
            print("No timetable was found")

    def stringify(self):
        s = """Timetable for route """ + self.route.stringify()
        if self.timetable:
            for journey in self.timetable:
                s += journey.stringify()
            s += """\n"""
        else:
            s += "No timetable was found"
        return s
            
class Timetables:
    def __init__(self, routes):
        self.routes = routes
        self.timetables = []
        scraper = SeleniumScraper()
        for route in self.routes:
            self.timetables.append(Timetable(scraper, route))
        scraper.destroy
    
    def _print(self):
        for timetable in self.timetables:
            timetable._print()

class SeleniumScraper:
    def __init__(self):
        options = Options()
        options.headless = True

        DRIVER_PATH = "/opt/bin/chromedriver"
        self.driver = webdriver.Chrome(options=options, executable_path=DRIVER_PATH)
        self.driver.implicitly_wait(5)

    def destroy(self):
        self.driver.close()
    
    def scrape(self, route):
        URL = "https://www.ostgotatrafiken.se/"
        URL += route.from_station.URL_id + "/" + route.to_station.URL_id
        self.driver.get(URL)
        time.sleep(2)

        xpath_expr = "//div[@class='departure__item accordion_list__item clearfix expandable main']"
        html_table = self.driver.find_elements_by_xpath(xpath_expr)

        timetable = []
        for table_entry in html_table:
            l = table_entry.text.split("\n")

            if len(l) < 4:
                continue

            l = l[:-1] if l[-1] == "Öppna" else l
            
            deviated_time = True if "-" in l[1] else False

            travel_plan = "X" if len(l) - deviated_time < 4 else l[3 + deviated_time]
            travel_time = l[1 + deviated_time]
            switches = l[2 + deviated_time].split(" ")[0]
            dep_time = l[0].split("-")[0].strip()
            arr_time = l[0].split("-")[1].strip()

            new_dep_time = l[1].split("-")[0].strip() if deviated_time else dep_time
            new_arr_time = l[1].split("-")[1].strip() if deviated_time else arr_time

            journey = Journey([dep_time, arr_time],
                              [new_dep_time, new_arr_time],
                              travel_time,
                              travel_plan,
                              switches,
                              deviated_time)
            timetable.append(journey)

        return timetable

class TrafficTracker:
    def __init__(self):
        stations = [
            Station("Matteusskolan", "matteusskolan"),
            Station("Nkpg Resecentrum", "norrkopings-resecentrum"),
            Station("Hagaskolan", "hagaskolan-norrkoping"),
            Station("Lkpg Resecentrum", "linkopings-resecentrum"),
        ]
    
        routes_to_track = [Route(stations[0], stations[1]),
                           #Route(stations[0], stations[2]),
                           Route(stations[1], stations[3]),
        ]

        self.t = Timetables(routes_to_track)
        
    def print_timetables(self):
        self.t._print()
        
    def gui_add(self, gui):
        print_on_row = 15
        print_on_col = 80
        
        gui.add("Timetables", ("Arial Bold", 12), (print_on_col, print_on_row))
        print_on_row += 45
        
        for k in range(len(self.t.timetables)):
            timetable = self.t.timetables[k]
            gui.add(timetable.route.stringify(), ("Arial Bold", 10), (print_on_col, print_on_row))
            print_on_row += 20
            for i in range(len(timetable.timetable)):
                journey = timetable.timetable[i]
                gui.add(journey.stringify_minimal(), ("Arial Bold", 10), (print_on_col, print_on_row))
                print_on_row += 20
            print_on_row += 20
