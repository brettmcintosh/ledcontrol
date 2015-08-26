from collections import OrderedDict

#parameters = OrderedDict({'rainbow': [('interval', 0, 30), ('brightness', 0, 255)],
#                          'sparkle': [('interval', 0, 50), ('brightness', 0, 255)], })
parameters = OrderedDict({'rainbow':[	
										{'type':'number', 'name':'Interval', 'min':0, 'max':30, 'order':1, 'default':20}, 
										{'type':'number', 'name':'Brightness', 'min':0, 'max':255, 'order':2, 'default':255}, 
										{'type':'boolean', 'name':'Direction', 'min':0, 'max':1, 'order':3, 'default':1}],
						'theater_chase': [		
										{'type':'number', 'name':'Color', 'min':0, 'max':255, 'order':1, 'default':(0,255)}, 
										{'type':'number', 'name':'Color', 'min':0, 'max':255, 'order':1, 'default':(0,255)}, 
										{'type':'number', 'name':'Interval', 'min':0, 'max':30, 'order':1, 'default':(20,50)}, 
										{'type':'number', 'name':'Length', 'min':0, 'max':30, 'order':1, 'default':(1,10)}, 
										{'type':'number', 'name':'Brightness', 'min':0, 'max':255, 'order':2, 'default':255}, 
										{'type':'number', 'name':'Direction', 'min':0, 'max':1, 'order':3, 'default':1}],
						})

