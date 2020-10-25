#!/usr/bin/env python3

class Ex1(Model.models):
    name = 'ex1'

class Ex2(Model.models):
    name = 'ex2'


class Sub1Ex1(Model.models):
    inherit = "ex1"

class Sub2Ex1(Model.models):
    inherit ="ex1"

class Sub3Ex1(Model.models):
    inherit ='ex1'

class Sub4Ex1(Model.models):
    inherit="ex1"
    
class Sub1Ex2(Model.models):
    inherit="ex2"

    
