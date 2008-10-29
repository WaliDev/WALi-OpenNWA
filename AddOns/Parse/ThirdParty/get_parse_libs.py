#!/usr/bin/env python

## ###############
## @author kidd

import os
import platform
import shutil
import sys


#xerces_master   = 'http://www.apache.org/dist/xerces/c/2/binaries/'
#
#xerces_win32    = 'xerces-c_2_8_0-x86-windows-vc_8_0'
#xerces_win64    = 'xerces-c_2_8_0-x86_64-windows-vc_8_0'
#xerces_lin32  = 'xerces-c_2_8_0-x86-lin-gcc_3_4'
#xerces_lin64  = 'xerces-c_2_8_0-x86_64-lin-gcc_3_4'

xrcs3_mirror  = 'http://apache.cs.utah.edu/xerces/c/3/binaries/'
xrcs3_win32   = 'xerces-c_3_0_0-x86-windows-vc_9_0'
xrcs3_win64   = 'xerces-c_3_0_0-x86_64-windows-vc_9_0'
xrcs3_lin32   = 'xerces-c-3.0.0-x86-linux-gcc-3.4'
xrcs3_lin64   = 'xerces-c-3.0.0-x86_64-linux-gcc-3.4'

(bits,linkage)  = platform.architecture()
sys             = platform.system()
IsLinux         = sys == 'Linux'
Is64            = bits == '32bit'
name = (
    ( xrcs3_win64 , xrcs3_win32 ), 
    ( xrcs3_lin64 , xrcs3_lin32 )
    )[IsLinux][Is64]

def myrm(name):
  if os.path.isdir(name):
    os.system('rm -rf %s' % name)
  elif os.path.isfile(name):
    os.remove(name)

def getlibs(LibInstallDir):
  built = []
  if False == os.path.exists(name):
    target = name+('.zip','.tar.gz')[IsLinux]
    myrm(target)
    os.system('curl -O %s%s' % (xrcs3_mirror,target))
    os.system('%s %s' % (('unzip -o','tar zxf')[IsLinux],target))
  cwd = os.getcwd()
  if False == os.path.exists(LibInstallDir):
    os.mkdir(LibInstallDir)
  for lib in os.listdir(os.path.join(name,'lib')):
    src = os.path.join(cwd,name,'lib',lib)
    dst = os.path.join(LibInstallDir,lib)
    print 'Src = %s' % src
    print 'Dst = %s' % dst
    shutil.copy2(src,dst)
    built += [dst]
  return (name,built)


# Work from command line or SConscript
if __name__ in ['__main__']:
  if bits != '64bit' and bits != '32bit':
    raise NameError, bits
  Is64           = (False,True)[bits == '64bit']
  getlibs(LibInstallDir)


