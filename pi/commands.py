import array
from random import randrange


def create_command_array(initializer):
    initializer = [randrange(i[0], i [1]) if isinstance(i, tuple) else i for i in initializer]
    return array.array('B', initializer)


commands = {'rainbow': [1, 20, 255, 1],
            'theater': [2, (0, 255), (0, 255), (20, 50), (1, 10), 255, 1],
            'scanner': [3, (0, 255), 10, 255, 0],
            'fade': [4, 50, 250, 100, (5, 12), 1],
            'bounce': [5, 10, 255, 32],
            'sparkle': [6, (0, 255), (1, 15), 30, (175, 255)],
            'sparkle white': [6, 0, (1, 15), 0, 0],
            'random chase': [7, 25, 2, 8, 2, 8, 40, 1],
            'janky chase': [7, 25, 2, 6, 4, 8, 40, 0], }

command_parameters = {#'rainbow':[	
				#{'type':'range', 'name':'Interval', 'min':0, 'max':30, 'order':1, 'default':20}, 
				#{'type':'range', 'name':'Brightness', 'min':0, 'max':255, 'order':2, 'default':255}, 
				#{'type':'checkbox', 'name':'Reverse Direction', 'min':0, 'max':1, 'order':3, 'default':1}],
			'theater_chase': [		
				{'type':'range', 'name':'Color', 'min':0, 'max':255, 'order':1, 'default':50}, 
				{'type':'range', 'name':'Color', 'min':0, 'max':255, 'order':2, 'default':150}, 
				{'type':'range', 'name':'Interval', 'min':0, 'max':30, 'order':3, 'default':35}, 
				{'type':'range', 'name':'Gap Length', 'min':0, 'max':30, 'order':4, 'default':5}, 
				{'type':'range', 'name':'Brightness', 'min':0, 'max':255, 'order':5, 'default':255}, 
				{'type':'checkbox', 'name':'Reverse Direction', 'min':0, 'max':1, 'order':6, 'default':1}],
			}