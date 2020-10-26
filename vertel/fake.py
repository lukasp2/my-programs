#!/usr/bin/env python3

class Ex1(Model.models):
    _name = 'ex1'

class Ex2(Model.models):
    _name = 'ex2'
    

class Sub1Ex1(Model.models):
    _inherit = "ex1"

class Sub2Ex1(Model.models):
    _inherit ="ex1"

class Sub3Ex1(Model.models):
    _inherit ='ex1'

class Sub4Ex1(Model.models):
    _inherit="ex1"
    
class Sub1Ex2(Model.models):
    _inherit="ex2"
