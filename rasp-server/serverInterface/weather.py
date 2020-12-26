#!/usr/bin/env python3

import os

class Weather():
    def __init__(self):
        cities = ["Norrköping", "Linköping"]
        self.weather_reports = []
        for city in cities:
            bash_command = "curl -s wttr.in/" + city + " | head -n 7 | tail -n 7 | sed 's/\x1b\[[^\x1b]*m//g'"
            output = os.popen(bash_command).read()
            output = output.split('\n')
            for i in range(len(output)):
                output[i] = output[i][16:]
            self.weather_reports.append([line.strip() for line in output if line.strip()])
            
    def _print(self):
        for report in self.weather_reports:
            for stat in report:
                print(stat)

    def gui_add(self, gui):
        print_on_row = 15
        print_on_col = 400

        gui.add(text="Weather", font=("Arial Bold", 12), position=(print_on_col, print_on_row))
        print_on_row += 40
        for report in self.weather_reports:
            for stat in report:
                gui.add(text=stat, font=("Arial Bold", 10), position=(print_on_col, print_on_row))
                print_on_row += 20
            print_on_row += 20
            
