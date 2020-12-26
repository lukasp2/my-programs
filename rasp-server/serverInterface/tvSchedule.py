#!/usr/bin/env python3

class ChannelScraper():
    def __init__(self, channel_name):
        pass
        
class ChannelSchedule():
    def __init__(self, name):
        schedule = ChannelScraper(name)

    def stringify(self):
        pass
        
class TvSchedule():
    def __init__(self):
        track_channels = ["SVT1","SVT2","TV3"]
        self.schedules = [ChannelSchedule(channel) for channel in track_channels]
        
    def gui_add(self, gui):
        print_on_row = 15
        print_on_col = 600

        gui.add(text="TV", font=("Arial Bold", 12), position=(print_on_col, print_on_row))
        print_on_row += 20
        
        for schedule in self.schedules:
            gui.add(text=schedule.stringify(), font=("Arial Bold", 10), position=(print_on_col, print_on_row))
            print_on_row += 20
