#!/usr/bin/env python
## ###############
## @author kidd
##
## Separate out Xerces constants
## so that they can be easily
## found by other apps.
import platform

Xerces = {
    'Darwin' : ('xerces-c-3.1.1-x86-macosx-gcc-4.0',
                'xerces-c-3.1.1-x86-macosx-gcc-4.0'),
    'Linux' : ('xerces-c-3.1.1-x86-linux-gcc-3.4',
               'xerces-c-3.1.1-x86_64-linux-gcc-3.4'),
    'Windows' : ('xerces-c-3.1.1-x86-windows-vc-9.0',
                'xerces-c-3.1.1-x86_64-windows-vc-9.0')
    }


(bits,linkage)  = platform.architecture()
sys             = platform.system()
IsZip           = sys == 'Windows'

def get_name(is64):
    return Xerces[sys][is64]


