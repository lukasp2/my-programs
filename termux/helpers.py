#!/usr/bin/env python3

def get_input(num_options):
    print("Enter option ...")
    opt = int(input())
    
    while opt < 0 or opt > num_options:
        print("Invalid input, try again ...")
        opt = int(input())
        
    return opt

def print_prompt(prompt):
    colors.Green + prompt[0] + colors.NC
    for i in range(1, len(prompt)):
        print(str(i - 1) + "." + prompt[i])
    print()
