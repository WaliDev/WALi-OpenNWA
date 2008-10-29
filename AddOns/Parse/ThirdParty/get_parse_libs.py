#!/usr/bin/python

## ###############
## @author kidd

import platform
import urllib
import sys
import os


#xerces_master   = 'http://www.apache.org/dist/xerces/c/2/binaries/'
#
#xerces_win32    = 'xerces-c_2_8_0-x86-windows-vc_8_0'
#xerces_win64    = 'xerces-c_2_8_0-x86_64-windows-vc_8_0'
#xerces_linux32  = 'xerces-c_2_8_0-x86-linux-gcc_3_4'
#xerces_linux64  = 'xerces-c_2_8_0-x86_64-linux-gcc_3_4'

xrcs3_mirror    = 'http://apache.cs.utah.edu/xerces/c/3/binaries/'
xrcs3_win32     = 'xerces-c_3_0_0-x86-windows-vc_9_0'
xrcs3_win64     = 'xerces-c_3_0_0-x86_64-windows-vc_9_0'
xrcs3_linux32   = 'xerces-c-3.0.0-x86-linux-gcc-3.4'
xrcs3_linux64   = 'xerces-c-3.0.0-x86_64-linux-gcc-3.4'

(bits,linkage)  = platform.architecture()
sys             = platform.system()
name = ((xrcs3_win64,xrcs3_win32), (xrcs3_linux64 , xrcs3_linux32))[sys == 'Linux'][bits == '32bit']

def myrm(name):
    if os.path.isdir(name):
        os.system('rm -rf %s' % name)
    elif os.path.isfile(name):
        os.remove(name)

def xerces_setup(name):
    myrm(name)
    myrm(XercesTgt)

def get_xerces_win(XercesTgt):
    target = name+'.zip'
    xerces_setup(target)
    os.system('curl -O %s%s' % (xrcs3_mirror,target))
    os.system('unzip -o %s' % target)
    os.rename(name, XercesTgt)

def get_xerces_linux(XercesTgt):
    target = name+'.tar.gz'
    xerces_setup(target)
    os.system('curl -O %s%s' % (xrcs3_mirror,target))
    os.system('tar zxf %s' % target)
    os.rename(name, XercesTgt)

def getlibs(XercesTgt):
  if False == os.path.exists(XercesTgt):
    if s == 'Linux':
      get_xerces_linux(XercesTgt)
    else:
      get_xerces_win(XercesTgt)


# Work from command line or SConscript
if __name__ in ['__main__']:
  if bits != '64bit' and bits != '32bit':
    raise NameError, bits
  Is64           = (False,True)[bits == '64bit']
  XercesTgt      = ('xerces-c','xerces-c64')[Is64]
  getlibs(XercesTgt)


