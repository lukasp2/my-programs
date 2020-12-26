#!/usr/bin/env python3

import trafficTracker
import gui
import weather
import tvSchedule
import flowers

def main():
    _gui = gui.Gui()
    
    tt = trafficTracker.TrafficTracker()    
    tt.gui_add(_gui)
    #tt.print_timetables()

    tv = tvSchedule.TvSchedule()
    tv.gui_add(_gui)

    h2o = flowers.Watering()
    h2o.gui_add(_gui)
    
    wt = weather.Weather()
    wt.gui_add(_gui)
    
    _gui.show()

main()    

"""
requirements:
$ pip3 install selenium bs4

for selenium: 
sudo apt install google-chrome
+ chrome driver

sudo apt install python3-tkinter
"""

