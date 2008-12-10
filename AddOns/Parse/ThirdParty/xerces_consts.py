#!/usr/bin/env python
## ###############
## @author kidd
##
## Separate out Xerces constants
## so that they can be easily
## found by other apps.
import platform

xrcs3_win32   = 'xerces-c-3.0.0-x86-windows-vc-9.0'
xrcs3_win64   = 'xerces-c-3.0.0-x86_64-windows-vc-9.0'
xrcs3_lin32   = 'xerces-c-3.0.0-x86-linux-gcc-3.4'
xrcs3_lin64   = 'xerces-c-3.0.0-x86_64-linux-gcc-3.4'

(bits,linkage)  = platform.architecture()
sys             = platform.system()
IsLinux         = sys == 'Linux'
Is64            = bits == '32bit'
Name = (
    ( xrcs3_win64 , xrcs3_win32 ), 
    ( xrcs3_lin64 , xrcs3_lin32 )
    )[IsLinux][Is64]


