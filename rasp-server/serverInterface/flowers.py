#!/usr/bin/env python3

class Watering:
    def __init__(self):
        self.waterSchedule = {}

    def gui_add(self, gui):
        print_on_row = 15
        print_on_col = 800

        gui.add("Flowers", ("Arial Bold", 12), (print_on_col, print_on_row))

