import array
from random import randrange


def create_command_array(initializer):
    initializer = [randrange(i[0], i [1]) if isinstance(i, tuple) else i for i in initializer]
    return array.array('B', initializer)


commands = {'rainbow': [1, 20, 255, 1],
            'theater': [2, 100, 250, 10, (5, 10), 255, 1],
            'scanner': [3, (0, 255), 10, 255, 0],
            'fade': [4, 50, 250, 100, 5, 1],
            'bounce': [5, 10, 255, 0]}