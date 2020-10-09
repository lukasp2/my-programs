#!/usr/bin/env python3

import os
import colors

class Menu:
    def __init__(self, header, options):
        self.header = header
        self.option = options
    
    def print_menu_prompt(self):
        colors.Green + self.header + colors.NC
        for i in range(len(self.options)):
            print(str(i) + "." + self.options[i])
        print()

    def get_menu_input(self):
        print("Enter option ...")
        opt = int(input())
    
        while opt < 0 or opt >= len(self.options):
            print("Invalid input, try again ...")
            opt = int(input())
        
        return opt

    # abstract class, force override:
    def handle_input():
        pass

    def run(self):
        # main loop here?
        
        self.print_menu_prompt()
        option = self.get_menu_input()
        handle_input(option)

        return new_menu

class Main_Menu(Menu):
    def __init__(self):
        header = "## Welcome to the System Menu ##"
        options = ["exit ssh", \
                   "system control", \
                   "access browser"]

        Menu.__init__(self, header, options)

    def handle_input(option):
        if option == 0:
            return
        elif option == 1:
            return System_Menu()
        elif option == 2:
            return Browser_Menu()
        else:
            print("An error occured ...")
            return self
        
class System_Menu(Menu):
    def __init__(self):
        header = "## Welcome to the System Menu ##"
        options = ["back to main menu", \
                  "volume control", \
                  "system reboot", \
                  "system shutdown"]
        Menu.__init__(self, header, options)

    def handle_input(self, option):
        if option == 0:
            return Main_Menu()
        elif option == 1:
            # implement system volume control
            # return Volume_Menu() ??
            pass
        elif option == 2:
            print("rebooting system ...")
            os.system("sudo reboot")
        elif option == 3:
            print("shutting system down ...")
            os.system("sudo shutdown now")
        else:
            print("An error occured ...")
            
        return self

class Browser_Menu(Menu):
    def __init__(self):
        header = "## Welcome to the Browser Menu ##"
        options = ["back to main menu"]
        self.links = [("youtube", "https://www.youtube.com/"),
                      ("nyafilmer", "https://nyafilmer.vip/")]
        Menu.__init__(self, header, options)

    def print_menu_prompt(self):
        colors.Green + self.header + colors.NC
        for i in range(len(self.options)):
            print(str(i) + "." + self.options[i])

        for i in range(len(self.options), len(self.links)):
            print(str(i) + "." + self.links[i][0])
        print()

    def handle_input(self, option):
        if option == 0:
            return Main_Menu()
        elif option == 1:
            print("going to youtube or whatev ...")
            #return Browser_Youtube() ??
            #firefox-esr ....
        else:
            print("An error occured ...")

        return self
        
m = Main_Menu()
m.run()

