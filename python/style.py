"""Provides functions for setting the print style of the command line
"""
from enum import Enum
import re

__BASE_FORMAT = "\033[{}m{}\033[0m"

class StyleError(Exception):
    pass

class BaseColor(Enum):
    BLACK = (30)
    RED = (31)
    GREEN = (32)
    YELLOW = (33)
    BLUE = (34)
    MAGENTA = (35)
    CYAN = (36)
    WHITE = (37)

    DEFAULT = (39)

    def __init__(self, val):
        self._value_ = val

class ColorType(Enum):
    SIMPLE_8 = 1
    CUSTOM_256 = 2
    CUSTOM_RGB = 3

class Color:    
    def __init__(self, color_type, value):
        if(color_type == ColorType.SIMPLE_8):
            self.color_type = color_type
            if(type(value) == int):
                if((value >= 30 and value <= 37) or value == 39):
                    self.color = [value]
                else:
                    raise StyleError("SIMPLE_8 Color must be within [30,39] excluding 38")
            else:
                raise StyleError("SIMPLE_8 Color type must be int")
        elif(color_type == ColorType.CUSTOM_256):
            self.color_type = color_type
            if(type(value) == int):
                if(value >= 0 and value <= 255):
                    self.color = [38, 5, value]
                else:
                    raise StyleError("CUSTOM_256 Color must be within [0,255]")
            else:
                raise StyleError("CUSTOM_256 Color type must be int")
        elif(color_type == ColorType.CUSTOM_RGB):
            self.color_type = color_type
            pass
        else:
            raise StyleError("Invalid Color Type")

    def get_code(self):
        return self.color
    




class Style:
    def __init__(self):
        self.styles = []

    
    def format(self, text):
        if len(self.styles) == 0:
            return text
        else:
            return __BASE_FORMAT.format(';'.join(self.styles), text)
            