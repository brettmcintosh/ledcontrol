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
