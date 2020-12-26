from tkinter import *

class Gui():
    def __init__(self):
        self.window = Tk()
        self.window.title("Server Interface")
        self.window.geometry('1080x720')

    def add(self, text="ABC", font=("Arial Bold", 10), position=(0,0)):
        lbl = Label(self.window, text=text, font=font)
        lbl.place(x=position[0], y=position[1])

    def show(self):
        self.window.mainloop()
